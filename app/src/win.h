/*
 * @file   win.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

#include <stdbool.h>

#include "common.h"

struct win {
        void* p;
        enum color* grid;
        struct vec2 pos;
        struct vec2 dim;
        const bool border;
};

int win_create(struct win* const w, bool use_grid);
void win_print(struct win* const w, const char* fmt, ...);
void win_put(struct win* const w, const struct vec2* const p, u32 c);
void win_destroy(struct win* const w);
void win_refresh(struct win* const w);

static inline enum color* const win_at(struct win* const w, u32 row, u32 col)
{
        return &w->grid[row * w->dim.row + col];
}

static inline enum color const win_atc(const struct win* const w, u32 row, u32 col)
{
        return w->grid[row * w->dim.row + col];
}

static inline bool win_in_bounds(const struct win* const w, u32 row, u32 col)
{
        return (row < (w->dim.row - 1)) && (col > 0) && (col < (w->dim.col - 1));
}

#endif /* WIN_H_INCLUDED */

/**** END OF FILE ****/
