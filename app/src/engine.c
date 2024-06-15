/*
 * @file   engine.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <ncurses.h>
#include <stdlib.h>

#include "common.h"

static const u8 colors[] = {
        COLOR_CYAN,  COLOR_YELLOW, COLOR_BLUE,  COLOR_MAGENTA,
        COLOR_GREEN, COLOR_RED,    COLOR_WHITE, COLOR_BLACK,
};

static int engine_col_init()
{
        const bool has = has_colors();
        if (has == FALSE)
                return ERR;

        int err = use_default_colors();
        if (err == ERR)
                return ERR;

        err = start_color();
        if (err == ERR)
                return ERR;

        for (u8 i = 0; i < ARR_SIZE(colors); i++) {
                err = init_pair(i + 1, colors[i], -1);
                if (err == ERR)
                        return ERR;
        }

        return OK;
}

int engine_init(void)
{
        initscr();
        cbreak();
        keypad(stdscr, 1);
        noecho();
        curs_set(0);
        timeout(1);

        atexit((void (*)(void))endwin);

        int error = engine_col_init();
        if (error == -1)
                return -1;

        refresh();

        return 0;
}

/**** END OF FILE ****/
