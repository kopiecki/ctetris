/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "engine.h"
#include "tet.h"
#include "win.h"

#define KEY_ESC 27

static struct win board = {
        .dim    = { 50, 50 },
        .pos    = { 0, 0 },
        .border = true,
};

static const struct vec2 up    = { 0, -1 };
static const struct vec2 down  = { 0, 1 };
static const struct vec2 left  = { -1, 0 };
static const struct vec2 rigth = { 1, 0 };

inline static int get_rn(int lower, int upper)
{
        return (rand() % (upper - lower + 1)) + lower;
}

inline static enum color get_rand_color()
{
        return get_rn(color_cyan, color_red);
}

inline static enum tet_tp get_rand_tet_tp()
{
        return get_rn(tet_tp_i, tet_tp_z);
}

inline static void sleep_ms(u32 ms)
{
        usleep(ms * 1000);
}

int main(void)
{
        int err = engine_init();
        if (err == ERR)
                return ERR;

        err = win_create(&board);
        if (err == ERR)
                return ERR;

        srand(time(NULL));

        struct tet t = tet_create(get_rand_tet_tp(), get_rand_color());

        bool moved = false;
        i32 delay  = INT16_MAX;

        while (1) {
                delay -= 800;
                if (delay < 0) {
                        moved = tet_move_down(&t, &board);
                        delay = INT16_MAX;
                }

                switch (getch()) {
                        case KEY_LEFT:
                                tet_move_left(&t, &board);
                                break;
                        case KEY_RIGHT:
                                tet_move_right(&t, &board);
                                break;
                        default:
                                break;
                }

                if (!moved) {
                        t = tet_create(get_rand_tet_tp(), get_rand_color());
                }

                win_refresh(&board);
        }

        return OK;
}

/**** END OF FILE ****/
