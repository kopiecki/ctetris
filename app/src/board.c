/*
 * @file   board.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "board.h"

#include <assert.h>
#include <ncurses.h>

#include "common.h"

static inline void board_put(const struct board* const b, struct vec2 p)
{
        const enum color c = board_atc(b, p);
        if (c == color_none)
                win_put(b->w, EMPTY, p, c);
        else
                win_put(b->w, BLOCK, p, c);
}

int board_create(struct board* const b)
{
        assert(b != NULL);

        b->grid = calloc(b->w->dim.row * b->w->dim.col, sizeof(enum color));
        if (b->grid == NULL)
                return ERR;

        return OK;
}

void board_render(const struct board* const b)
{
        assert(b != NULL);
        assert(b->grid != NULL);
        assert(b->w != NULL);

        struct vec2 p = { 1, 1 };
        for (; p.row < b->w->dim.row - 1; p.row++)
                for (p.col = 1; p.col < b->w->dim.col - 1; p.col++)
                        board_put(b, p);
}

/**** END OF FILE ****/
