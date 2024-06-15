/*
 * @file   win.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

#include <ncurses.h>
#include <stdbool.h>

#include "common.h"

struct win {
        void* p;
        enum color* grid;
        struct vec2 pos;
        struct vec2 dim;
        const bool border;
};

int win_create(struct win* const w);
void win_destroy(struct win* const w);
void win_refresh(struct win* const w);

static inline enum color* const win_at(struct win* const w, u32 row, u32 col)
{
        return &w->grid[row * w->dim.row + col];
}

#endif /* WIN_H_INCLUDED */

/**** END OF FILE ****/
