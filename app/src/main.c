/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

#include "common.h"
#include "engine.h"
#include "tet.h"
#include "win.h"

#define DELAY_INIT INT16_MAX
#define DELAY_STEP 1000
#define KEY_ESC    27

int main(void)
{
        int err = engine_init();
        if (err == ERR)
                return ERR;

        struct win board = {
                .dim    = { 50, 50 },
                .pos    = { 0, 0 },
                .border = true,
        };

        struct win score = {
                .dim    = { 4, 20 },
                .pos    = { 1, board.dim.col + 1 },
                .border = false,
        };

        struct win next = {
                .dim    = { 8, 10 },
                .pos    = { score.dim.row - 1, board.dim.col + 4 },
                .border = false,
        };

        struct win ctrl = {
                .dim    = { 10, 25 },
                .pos    = { next.dim.row + 2, board.dim.col + 1 },
                .border = false,
        };

        struct win logs = {
                .dim    = { 10, 50 },
                .pos    = { board.dim.row, 0 },
                .border = false,
        };

        err = win_create(&board, true);
        if (err == ERR) {
                return ERR;
        }

        err = win_create(&next, true);
        if (err == ERR) {
                return ERR;
        }

        err = win_create(&score, false);
        if (err == ERR) {
                return ERR;
        }

        err = win_create(&ctrl, false);
        if (err == ERR) {
                return ERR;
        }

        err = win_create(&logs, false);
        if (err == ERR) {
                return ERR;
        }

        win_print(
            &ctrl,
            "a/left/h: move left\n"
            "d/right/l: move right\n"
            "w/up/j: rotate\n"
            "space: hard drop");

        struct tet t0 = tet_spawn(&board);
        struct tet t1 = tet_spawn(&next);

        tet_draw(&t1, &next);

        bool moved    = true;
        i32 delay     = DELAY_INIT;
        u32 score_cnt = 0;

        while (true) {
                delay -= DELAY_STEP;
                if (delay < 0) {
                        moved = tet_move(&t0, &board, dir_down);
                        delay = INT16_MAX;
                }

                switch (getch()) {
                        case 'a':
                        case 'h':
                        case KEY_LEFT:
                                tet_move(&t0, &board, dir_left);
                                break;
                        case 'd':
                        case 'l':
                        case KEY_RIGHT:
                                tet_move(&t0, &board, dir_right);
                                break;
                        case 'w':
                        case 'j':
                        case KEY_UP:
                                tet_rotate(&t0, &board);
                                break;
                        default:
                                break;
                }

                if (!moved) {
                        tet_respawn(&board, &t0, &t1);
                        tet_erase(&t1, &next);
                        t1 = tet_spawn(&next);
                        tet_draw(&t1, &next);
                        moved = true;
                }

                win_refresh(&board);
                win_refresh(&next);
                win_refresh(&score);
                win_refresh(&logs);
                win_refresh(&ctrl);
        }

        return OK;
}

/**** END OF FILE ****/
