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

static WINDOW* mkwin(struct vec2 dim, struct vec2 pos, bool border)
{
        WINDOW* w = newwin(dim.row, dim.col, pos.row, pos.col);
        if (w == NULL)
                return NULL;

        if (border)
                if (box(w, 0, 0) == ERR)
                        return NULL;

        if (wrefresh(w) == ERR)
                return NULL;

        return w;
}

static int init_col(void)
{
        if (has_colors() == FALSE)
                return ERR;

        if (use_default_colors() == ERR)
                return ERR;

        if (start_color() == ERR)
                return ERR;

        for (u8 i = 0; i < ARR_SIZE(colors); i++)
                if (init_pair(i + 1, colors[i], -1) == ERR)
                        return ERR;

        return OK;
}

static int init_env(void)
{
        if (initscr() == NULL)
                return ERR;

        if (cbreak() == ERR)
                return ERR;

        if (keypad(stdscr, 1) == ERR)
                return ERR;

        if (noecho() == ERR)
                return ERR;

        if (curs_set(0) == ERR)
                return ERR;

        timeout(1);
        srand(time(NULL));

        if (atexit((void (*)(void))endwin) == ERR)
                return ERR;

        if (init_col() == ERR)
                return ERR;

        return wrefresh(stdscr);
}

static int create_wins(void)
{
        for (u8 i = 0; i < num_wins; i++) {
                wins[i] = mkwin(dims[i], poses[i], !i);
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
        for (u8 i = 0; i < num_wins; i++)
                if (wrefresh(wins[i]) == ERR)
                        return ERR;

        return OK;
}

static int create_boards(void)
{
        for (u8 i = 0; i < num_boards; i++) {
                boards[i] = (struct board) { .grid = NULL, .w = wins[i] };
                if (board_create(&boards[i]) == ERR)
                        return ERR;
        }

        return OK;
}

static int render_boards(void)
{
        for (u8 i = 0; i < num_boards; i++)
                if (board_render(&boards[i]) == ERR)
                        return ERR;

        return OK;
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
        if (wclear(wins[wscore]) == ERR)
                return ERR;

        return wprintw(wins[wscore], "score: %d", cnt);
}

static int update(void)
{
        if (render_boards() == ERR)
                return ERR;

        if (refresh_wins() == ERR)
                return ERR;

        return OK;
}

static void cleanup()
{
        destroy_wins();
        destroy_boards();
}

int game_loop(void)
{
        wprintw(wins[wctrl], "%s", ctrl_txt);

        struct tet on_game = tet_spawn(&boards[bgame]);
        struct tet on_next = tet_spawn(&boards[bnext]);

        tet_draw(&on_next, &boards[bnext]);

        i32 delay     = DELAY_INIT;
        u32 score_cnt = 0;

        if (print_score(score_cnt) == ERR)
                goto exit;

        while (true) {
                bool fell = true;

                delay -= DELAY_STEP;
                if (delay < 0) {
                        fell  = tet_move(&on_game, &boards[bgame], dir_down);
                        delay = INT16_MAX;
                }

                switch (getch()) {
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
                case 'j':
                case KEY_UP:
                        tet_rotate(&on_game, &boards[bgame]);
                        break;
                case KEY_ESC:
                        goto exit;
                default:
                        break;
                }

                if (!fell) {
                        swap_tets(&on_game, &on_next);
                        if (print_score(score_cnt) == ERR)
                                goto exit;
                }

                if (update() == ERR)
                        goto exit;
        }

exit:
        cleanup();

        return OK;
}

int main(int argc, char* argv[])
{
        if (init_env() == ERR)
                return ERR;

        if (create_wins() == ERR)
                return ERR;

        if (create_boards() == ERR)
                return ERR;

        return game_loop();
}

/**** END OF FILE ****/
