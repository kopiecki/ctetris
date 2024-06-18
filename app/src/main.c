/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

#include "board.h"
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

        struct win game = {
                .dim    = { 50, 50 },
                .pos    = { 0, 0 },
                .border = true,
        };

        struct win score = {
                .dim    = { 4, 20 },
                .pos    = { 1, game.dim.col + 1 },
                .border = false,
        };

        struct win next = {
                .dim    = { 8, 10 },
                .pos    = { score.dim.row - 1, game.dim.col + 4 },
                .border = false,
        };

        struct win ctrl = {
                .dim    = { 10, 25 },
                .pos    = { next.dim.row + 2, game.dim.col + 1 },
                .border = false,
        };

        struct win logs = {
                .dim    = { 10, 50 },
                .pos    = { game.dim.row, 0 },
                .border = false,
        };

        struct board gameb, nextb;

        err = win_create(&game);
        if (err == ERR) {
                return ERR;
        }

        err = board_create(&gameb, &game);
        if (err == ERR)
                return ERR;

        err = win_create(&next);
        if (err == ERR)
                return ERR;

        err = board_create(&nextb, &next);
        if (err == ERR)
                return ERR;

        err = win_create(&score);
        if (err == ERR)
                return ERR;

        err = win_create(&ctrl);
        if (err == ERR)
                return ERR;

        err = win_create(&logs);
        if (err == ERR)
                return ERR;

        win_print(
            &ctrl,
            "a/left/h: move left\n"
            "d/right/l: move right\n"
            "w/up/j: rotate\n"
            "space: hard drop");

        struct tet t0 = tet_spawn(&gameb);
        struct tet t1 = tet_spawn(&nextb);

        tet_draw(&t1, &nextb);

        bool moved    = true;
        i32 delay     = DELAY_INIT;
        u32 score_cnt = 0;

        while (true) {
                delay -= DELAY_STEP;
                if (delay < 0) {
                        moved = tet_move(&t0, &gameb, dir_down);
                        delay = INT16_MAX;
                }

                switch (getch()) {
                        case 'a':
                        case 'h':
                        case KEY_LEFT:
                                tet_move(&t0, &gameb, dir_left);
                                break;
                        case 'd':
                        case 'l':
                        case KEY_RIGHT:
                                tet_move(&t0, &gameb, dir_right);
                                break;
                        case 'w':
                        case 'j':
                        case KEY_UP:
                                tet_rotate(&t0, &gameb);
                                break;
                        default:
                                break;
                }

                if (!moved) {
                        tet_respawn(&gameb, &t0, &t1);
                        tet_erase(&t1, &nextb);
                        t1 = tet_spawn(&nextb);
                        tet_draw(&t1, &nextb);
                        moved = true;
                }

                board_render(&gameb);
                board_render(&nextb);

                win_refresh(&game);
                win_refresh(&next);
                win_refresh(&score);
                win_refresh(&logs);
                win_refresh(&ctrl);
        }

        return OK;
}

/**** END OF FILE ****/
