/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <curses.h>
#include <stdlib.h>

typedef int32_t i32;
typedef uint32_t u32;

struct win_cfg {
        i32 h, w;
        i32 x, y;
};

static const struct win_cfg board_cfg = {
        .h = 50,
        .w = 50,
        .x = 0,
        .y = 0,
};



static void engine_create(void)
{
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        atexit((void (*)(void))endwin);
        refresh();

        WINDOW* board = win_create(&board_cfg, true);

        while (1) {
        }
}

int main(void)
{
        engine_create();
        return 0;
}

/**** END OF FILE ****/
