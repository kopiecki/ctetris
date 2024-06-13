/*
 * @file   win.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

#include <curses.h>

#include "common.h"
#include "vec2.h"

struct win_cfg {
        struct vec2 dim;
        struct vec2 pos;
        bool border;
};

WINDOW* win_create(const struct win_cfg* const cfg);
void win_destroy(WINDOW* const win);

#endif /* WIN_H_INCLUDED */

/**** END OF FILE ****/
