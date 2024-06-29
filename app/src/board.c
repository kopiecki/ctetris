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
        struct vec2 p         = { 0, 0 };
        const struct vec2 max = get_limits(b->w);

        for (; p.row < max.row; p.row++)
                for (p.col = 1; p.col < max.col; p.col++)
                        *board_at(b, p) = CELL_EMPTY;
}

static int board_put(const struct board* const b, struct vec2 p)
{
        struct cell cl = board_atc(b, p);

        int err = wattron(b->w, COLOR_PAIR(cl.c));
        if (err < 0)
                return err;

        err = mvwaddch(b->w, p.row, p.col, cl.ch);
        if (err < 0)
                return err;

        err = wattroff(b->w, COLOR_PAIR(cl.c));
        if (err < 0)
                return err;

        return err;
}

int board_create(struct board* const b)
{
        assert(b != NULL);
        assert(b->w != NULL);

        const struct vec2 max = get_limits(b->w);

        b->grid = calloc(max.row, sizeof(struct cell*));
        if (b->grid == NULL)
                return ERR;

        for (u32 row = 0; row < max.row; row++) {
                b->grid[row] = calloc(max.col, sizeof(struct cell));
                if (b->grid[row] == NULL)
                        return ERR;
        }

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
        struct vec2 p = { row, 1 };

        const struct vec2 max  = get_limits(b->w);
        const struct vec2 down = { -1, 0 };

        for (; p.row > 2; p.row--)
                for (p.col = 1; p.col < max.col; p.col++)
                        *board_at(b, p) = board_atc(b, vec2_add_v(p, down));
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

        int err = OK;

        struct vec2 p         = { 1, 1 };
        const struct vec2 max = get_limits(b->w);

        for (; p.row < max.row; p.row++) {
                for (p.col = 1; p.col < max.col; p.col++) {
                        err = board_put(b, p);
                        if (err < 0)
                                return ERR;
                }
        }

        return OK;
}

/**** END OF FILE ****/
