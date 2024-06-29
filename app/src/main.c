/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "cfg.h"
#include "common.h"
#include "tet.h"

static WINDOW* wins[num_wins];
static struct board boards[num_boards];

static const u8 colors[] = {
        COLOR_CYAN,  COLOR_YELLOW, COLOR_BLUE,  COLOR_MAGENTA,
        COLOR_GREEN, COLOR_RED,    COLOR_WHITE, COLOR_BLACK,
};

static WINDOW* create_win(struct vec2 dim, struct vec2 pos, bool border)
{
        WINDOW* w = newwin(dim.row, dim.col, pos.row, pos.col);
        if (w == NULL)
                return NULL;

        int err = OK;
        if (border) {
                err = box(w, 0, 0);
                if (err < 0)
                        return NULL;
        }

        err = wrefresh(w);
        if (err < 0)
                return NULL;

        return w;
}

static int init_col(void)
{
        int err = ERR;
        if (has_colors() == FALSE)
                return err;

        err = use_default_colors();
        if (err < 0)
                return err;

        err = start_color();
        if (err < 0)
                return err;

        for (u8 i = 0; i < ARR_SIZE(colors); i++) {
                err = init_pair(i + 1, colors[i], -1);
                if (err < 0)
                        return err;
        }

        return err;
}

static int init_env(void)
{
        int err = ERR;
        if (initscr() == NULL)
                return err;

        err = cbreak();
        if (err < 0)
                return err;

        err = keypad(stdscr, 1);
        if (err < 0)
                return err;

        err = noecho();
        if (err < 0)
                return err;

        err = curs_set(0);
        if (err < 0)
                return err;

        timeout(1);
        srand(time(NULL));

        err = atexit((void (*)(void))endwin);
        if (err < 0)
                return err;

        err = init_col();
        if (err < 0)
                return err;

        err = wrefresh(stdscr);
        if (err < 0)
                return err;

        return err;
}

static int create_wins(void)
{
        for (u8 i = 0; i < num_wins; i++) {
                wins[i] = create_win(dims[i], poses[i], borders[i]);
                if (wins[i] == NULL)
                        return ERR;
        }

        return OK;
}

static void destroy_wins(void)
{
        for (u8 i = 0; i < num_wins; i++)
                delwin(wins[i]);
}

static int refresh_wins(void)
{
        int err = OK;
        for (u8 i = 0; i < num_wins; i++) {
                err = wrefresh(wins[i]);
                if (err < 0)
                        return err;
        }

        return err;
}

static int create_boards(void)
{
        int err = OK;
        for (u8 i = 0; i < num_boards; i++) {
                boards[i] = (struct board) { .grid = NULL, .w = wins[i] };
                err       = board_create(&boards[i]);
                if (err < 0)
                        return err;
        }

        return err;
}

static int render_boards(void)
{
        int err = OK;
        for (u8 i = 0; i < num_boards; i++) {
                err = board_render(&boards[i]);
                if (err < 0)
                        return err;
        }

        return err;
}

static void destroy_boards(void)
{
        for (u8 i = 0; i < num_boards; i++)
                board_destroy(&boards[i]);
}

static void swap_tets(struct tet* const on_main, struct tet* const on_sec)
{
        *on_main       = *on_sec;
        on_main->o.col = getmaxx(wins[wgame]) / 2 - 2;
        tet_erase(on_sec, &boards[bnext]);
        *on_sec = tet_spawn(&boards[bnext]);
        tet_draw(on_sec, &boards[bnext]);
}

static inline int print_score(u32 cnt)
{
        int err = mvwprintw(
                wins[wgame],
                dims[wgame].row - 1,
                dims[wgame].col / 2 - 4,
                "score: %d",
                cnt);
        if (err < 0)
                return err;

        return err;
}

static int update(void)
{
        int err = render_boards();
        if (err < 0)
                return err;

        err = refresh_wins();
        if (err < 0)
                return err;

        return err;
}

static void cleanup()
{
        destroy_wins();
        destroy_boards();
}

static int put_info(void)
{

        int err = mvwprintw(wins[wgame], 0, dims[wgame].col / 2 - 4, "ctetris");
        if (err < 0)
                return err;

        err = wprintw(wins[wctrl], "%s", ctrl_txt);
        if (err < 0)
                return err;

        err = mvwprintw(
                wins[wnext],
                dims[wnext].row - 1,
                dims[wnext].col / 2 - 2,
                "next");
        if (err < 0)
                return err;

        return err;
}

int game_loop(void)
{
        int err = put_info();
        if (err < 0)
                goto exit;

        struct tet on_game = tet_spawn(&boards[bgame]);
        struct tet on_next = tet_spawn(&boards[bnext]);

        tet_draw(&on_next, &boards[bnext]);

        i32 delay     = DELAY_INIT;
        i32 step      = DELAY_STEP;
        u32 score_cnt = 0;

        err = print_score(score_cnt);
        if (err < 0)
                goto exit;

        while (true) {
                bool fell = true;

                delay -= step;
                if (delay < 0) {
                        fell  = tet_move(&on_game, &boards[bgame], dir_down);
                        delay = INT16_MAX;
                }

                switch (getch()) {
                case 's':
                case 'j':
                case KEY_DOWN:
                        fell = tet_move(&on_game, &boards[bgame], dir_down);
                        break;
                case 'a':
                case 'h':
                case KEY_LEFT:
                        tet_move(&on_game, &boards[bgame], dir_left);
                        break;
                case 'd':
                case 'l':
                case KEY_RIGHT:
                        tet_move(&on_game, &boards[bgame], dir_right);
                        break;
                case 'w':
                case 'k':
                case KEY_UP:
                        tet_rotate(&on_game, &boards[bgame]);
                        break;
                case ' ':
                        tet_drop(&on_game, &boards[bgame]);
                        fell = false;
                        break;
                case '+':
                        step += DELAY_INC;
                        break;
                case '-':
                        step -= DELAY_INC;
                        break;
                case 'q':
                case KEY_ESC:
                        goto exit;
                default:
                        break;
                }

                if (!fell) {
                        swap_tets(&on_game, &on_next);
                        score_cnt += board_erase_rows(&boards[bgame]);
                        err = print_score(score_cnt);
                        if (err < 0)
                                goto exit;
                }

                err = update();
                if (err < 0)
                        goto exit;
        }

exit:
        cleanup();

        return err;
}

int main(int argc, char* argv[])
{
        int err = init_env();
        if (err < 0)
                return err;

        err = create_wins();
        if (err < 0)
                return err;

        err = create_boards();
        if (err < 0)
                return err;

        err = game_loop();
        if (err < 0)
                return err;

        return err;
}

/**** END OF FILE ****/
