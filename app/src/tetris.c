/*
 * @file   tetris.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TETRIS_H_INCLUDED_
#define TETRIS_H_INCLUDED_

#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

#include "board.h"
#include "cfg.h"
#include "common.h"
#include "engine.h"
#include "tet.h"
#include "win.h"

static inline struct win make_win(struct vec2 pos, struct vec2 dim, bool border)
{
        return (struct win) {
                .pos    = pos,
                .dim    = dim,
                .border = border,
        };
}

static struct win game, next, score, ctrl, logs;
static struct win* wins[] = { &game, &next, &score, &ctrl, &logs };

static struct board main, sec;
static struct board* boards[] = { &main, &sec };

static void _log(const char* fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        win_print(&logs, fmt, args);
        va_end(args);

        win_refresh(&logs);
}

static int create_wins(void)
{
        for (u8 i = 0; i < ARR_SIZE(wins); i++)
                if (win_create(wins[i]))
                        return ERR;

        return OK;
}

static int destroy_wins(void)
{
        for (u8 i = 0; i < ARR_SIZE(wins); i++)
                win_destroy(wins[i]);
}

static void refresh_wins(void)
{
        for (u8 i = 0; i < ARR_SIZE(wins) - 1; i++)
                win_refresh(wins[i]);
}

static int create_boards(void)
{
        for (u8 i = 0; i < ARR_SIZE(boards); i++)
                if (board_create(boards[i], wins[i]))
                        return ERR;

        return OK;
}

static void destroy_boards(void)
{
        for (u8 i = 0; i < ARR_SIZE(boards); i++)
                board_destroy(boards[i]);
}

static void render_boards(void)
{
        for (u8 i = 0; i < ARR_SIZE(boards); i++)
                board_render(boards[i]);
}

static void use_defaults(void)
{
        game  = make_win(wgame_pos, wgame_dim, true);
        score = make_win(wscore_pos, wscore_dim, false);
        next  = make_win(wnext_pos, wnext_dim, false);
        ctrl  = make_win(wctrl_pos, wctrl_dim, false);
        logs  = make_win(wlogs_pos, wlogs_dim, false);
}

static void parse_args(int argc, char* argv[])
{
        if (argc <= 1)
                use_defaults();
}

static void swap_tets(struct tet* const on_main, struct tet* const on_sec)
{
        *on_main       = *on_sec;
        on_main->o.col = main.w->dim.col / 2 - 2;
        tet_erase(on_sec, &sec);
        *on_sec = tet_spawn(&sec);
        tet_draw(on_sec, &sec);
}

int tetris_init(int argc, char* argv[])
{
        int err = engine_init();
        if (err == ERR)
                return ERR;

        parse_args(argc, argv);

        err = create_wins();
        if (err == ERR)
                return ERR;

        err = create_boards();
        if (err == ERR)
                return ERR;

        return OK;
}

int tetris_run(void)
{
        win_print(&ctrl, ctrl_txt);

        struct tet on_main = tet_spawn(&main);
        struct tet on_sec  = tet_spawn(&sec);

        tet_draw(&on_sec, &sec);

        i32 delay     = DELAY_INIT;
        u32 score_cnt = 0;

        win_print(&score, "score: %d", score_cnt);

        _log("Starting tetris game...");

        while (true) {
                bool fell = true;

                delay -= DELAY_STEP;
                if (delay < 0) {
                        fell  = tet_move(&on_main, &main, dir_down);
                        delay = INT16_MAX;
                }

                switch (getch()) {
                case 'a':
                case 'h':
                case KEY_LEFT:
                        tet_move(&on_main, &main, dir_left);
                        _log("Moving left...");
                        break;
                case 'd':
                case 'l':
                case KEY_RIGHT:
                        tet_move(&on_main, &main, dir_right);
                        _log("Moving right...");
                        break;
                case 'w':
                case 'j':
                case KEY_UP:
                        tet_rotate(&on_main, &main);
                        _log("Rotating...");
                        break;
                case KEY_ESC:
                        goto exit;
                default:
                        break;
                }

                if (!fell) {
                        swap_tets(&on_main, &on_sec);
                        win_print(&score, "score: %d", score_cnt);
                }

                render_boards();
                refresh_wins();
        }

exit:
        destroy_wins();
        destroy_boards();
        engine_deinit();

        return OK;
}

#endif /* TETRIS_H_INCLUDED_ */

/**** END OF FILE ****/
