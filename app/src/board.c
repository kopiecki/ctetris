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

static void reset(struct board* const b)
{
        struct vec2 p = { 0, 0 };
        for (; p.row < b->w->dim.row; p.row++)
                for (p.col = 0; p.col < b->w->dim.col; p.col++)
                        *board_at(b, p) = empty;
}

int board_create(struct board* const b, struct win* const w)
{
        assert(b != NULL);
        assert(w != NULL);

        b->grid = calloc(w->dim.row * w->dim.col, sizeof(struct cell));
        if (b->grid == NULL)
                return ERR;

        b->w = w;

        reset(b);

        return OK;
}

void board_destroy(struct board* const b)
{
        assert(b != NULL);

        free(b->grid);
}

void board_render(const struct board* const b)
{
        assert(b != NULL);
        assert(b->grid != NULL);
        assert(b->w != NULL);

        struct vec2 p = { 1, 1 };
        for (; p.row < b->w->dim.row - 1; p.row++)
                for (p.col = 1; p.col < b->w->dim.col - 1; p.col++)
                        win_put(b->w, p, board_atc(b, p));
}

/**** END OF FILE ****/
