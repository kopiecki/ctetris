/*
 * @file   board.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef BOARD_H_INCLUDED_
#define BOARD_H_INCLUDED_

#include <ncurses.h>

#include "common.h"

struct board {
        WINDOW* w;
        struct cell* grid;
};

int board_create(struct board* const b);
void board_destroy(struct board* const b);
int board_render(const struct board* const b);

static inline struct cell* const board_at(struct board* const b, struct vec2 p)
{
        return &b->grid[p.row * getmaxy(b->w) + p.col];
}

static inline struct cell const board_atc(const struct board* const b, struct vec2 p)
{
        return b->grid[p.row * getmaxy(b->w) + p.col];
}

static inline bool board_in(const struct board* const b, struct vec2 p)
{
        return (p.row < (getmaxy(b->w) - 1)) && (p.col > 0)
               && (p.col < (getmaxx(b->w) - 1));
}

#endif /* BOARD_H_INCLUDED_ */

/**** END OF FILE ****/
