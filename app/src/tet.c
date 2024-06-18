/*
 * @file   tet.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "tet.h"

#include <assert.h>
#include <curses.h>

#include "common.h"
#include "tet_srs_lut.h"
#include "win.h"

#define COL_STEP 2
#define ROW_STEP 1

#define TET_INIT_COL 20
#define TET_INIT_ROW 1

static inline bool tet_is(const struct tet* const t, struct vec2 p)
{
        return tet_srs_lut[t->tp][t->rot][p.row][p.col];
}

static inline struct cell tet_cell(enum color c)
{
        if (c == color_none)
                return empty;

        return (struct cell) {
                .ch = BLOCK,
                .c  = c,
        };
}

static void tet_fill_with(
    const struct tet* const t, struct board* const b, struct vec2 p, enum color c)
{
        assert(t != NULL);
        assert(b != NULL);

        if (tet_is(t, p))
                *board_at(b, vec2_add_v(p, t->o)) = tet_cell(c);
}

static bool tet_in_bounds(
    const struct tet* const t, const struct board* const b, struct vec2 p)
{
        assert(t != NULL);
        assert(b != NULL);

        const struct vec2 dpos = vec2_add_v(p, t->o);

        if (tet_is(t, p))
                if (!board_in(b, dpos) || board_atc(b, dpos).c != color_none)
                        return false;

        return true;
}

static bool tet_can_move(const struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        if (!tet_in_bounds(t, b, p))
                                return false;

        return true;
}

static void tet_move_by_dir(struct tet* const t, int d)
{
        assert(t != NULL);

        switch (d) {
                case dir_down:
                        t->o.row += ROW_STEP;
                        break;
                case dir_right:
                        t->o.col += COL_STEP;
                        break;
                case dir_left:
                        t->o.col -= COL_STEP;
                        break;
                case ~dir_down:
                        t->o.row -= ROW_STEP;
                        break;
                case ~dir_left:
                        t->o.col += COL_STEP;
                        break;
                case ~dir_right:
                        t->o.col -= COL_STEP;
                        break;
                default:
                        break;
        }
}

void tet_draw(const struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        tet_fill_with(t, b, p, t->c);
}

void tet_erase(const struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        tet_fill_with(t, b, p, color_none);
}

bool tet_move(struct tet* const t, struct board* const b, enum dir d)
{
        assert(t != NULL);
        assert(b != NULL);

        tet_erase(t, b);
        tet_move_by_dir(t, d);

        const bool can_move = tet_can_move(t, b);
        if (!can_move)
                tet_move_by_dir(t, ~d);

        tet_draw(t, b);

        return can_move;
}

void tet_rotate(struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        tet_erase(t, b);

        t->rot = (t->rot + 1) % 4;
        if (!tet_can_move(t, b))
                t->rot = (t->rot - 1) % 4;

        tet_draw(t, b);
}

struct tet tet_spawn(const struct board* const b)
{
        return (struct tet) {
                .tp = get_rn(tet_tp_i, tet_tp_z),
                .rot = rot_none,
                .c = get_rn(color_cyan, color_red),
                .o = {
                        .row = 1,
                        .col = b->w->dim.col / 2 - 4,
                },
        };
}

/**** END OF FILE ****/
