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

static inline struct vec2 get_limits(WINDOW* const w)
{
        return (struct vec2) {
                .row = getmaxy(w) - 1,
                .col = getmaxx(w) - 1,
        };
}

static inline bool is(struct board* const b, struct vec2 p)
{
        struct cell cl = board_atc(b, p);
        return (cl.c != CELL_EMPTY.c) && (cl.ch != CELL_EMPTY.ch);
}

static void reset(struct board* const b)
{
        struct vec2 p = { 0, 0 };
        for (; p.row < getmaxy(b->w) - 1; p.row++)
                for (p.col = 1; p.col < getmaxx(b->w) - 1; p.col++)
                        *board_at(b, p) = CELL_EMPTY;
}

static int board_put(const struct board* const b, struct vec2 p)
{
        struct cell cl = board_atc(b, p);

        if (wattron(b->w, COLOR_PAIR(cl.c)))
                return ERR;
        if (mvwaddch(b->w, p.row, p.col, cl.ch))
                return ERR;
        if (wattroff(b->w, COLOR_PAIR(cl.c)))
                return ERR;

        return OK;
}

int board_create(struct board* const b)
{
        assert(b != NULL);
        assert(b->w != NULL);

        b->grid = calloc(getmaxx(b->w) * getmaxy(b->w), sizeof(struct cell));
        if (b->grid == NULL)
                return ERR;

        reset(b);

        return OK;
}

void board_destroy(struct board* const b)
{
        assert(b != NULL);
        assert(b->grid != NULL);

        free(b->grid);
}

static void lower_down(struct board* const b, u32 row)
{
        struct vec2 p         = { row, 1 };
        const struct vec2 max = get_limits(b->w);

        for (; p.row > 2; p.row--) {
                for (p.col = 1; p.col < max.col; p.col++) {
                        const struct vec2 np = { .row = p.row - 1, .col = p.col };
                        *board_at(b, p)      = board_atc(b, np);
                }
        }
}

u8 board_erase_rows(struct board* const b)
{
        assert(b != NULL);
        assert(b->grid != NULL);
        assert(b->w != NULL);

        u8 num_erased = 0;

        struct vec2 p         = { 1, 1 };
        const struct vec2 max = get_limits(b->w);

        for (; p.row < max.row; p.row++) {
                for (p.col = 1; (p.col < max.col) && is(b, p); p.col++)
                        continue;

                if (p.col == max.col) {
                        lower_down(b, p.row);
                        num_erased++;
                }
        }

        return num_erased;
}

int board_render(const struct board* const b)
{
        assert(b != NULL);
        assert(b->grid != NULL);
        assert(b->w != NULL);

        struct vec2 p = { 1, 1 };
        for (; p.row < getmaxy(b->w) - 1; p.row++)
                for (p.col = 1; p.col < getmaxx(b->w) - 1; p.col++)
                        if (board_put(b, p))
                                return ERR;

        return OK;
}

/**** END OF FILE ****/
