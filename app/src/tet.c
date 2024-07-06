/*
 * @file   tet.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "tet.h"

#include <assert.h>
#include <curses.h>

#include "board.h"
#include "common.h"
#include "tet_srs_lut.h"

#define COL_STEP 2
#define ROW_STEP 1

#define TET_INIT_COL 20
#define TET_INIT_ROW 1

static inline bool is(const struct tet* const t, struct vec2 p)
{
        return tet_srs_lut[t->tp][t->rot][p.row][p.col];
}

static inline struct cell cell(enum color c)
{
        if (c == color_none)
                return CELL_EMPTY;

        return (struct cell) {
                .ch = BLOCK,
                .c  = c,
        };
}

static void fill_with(
        const struct tet* const t,
        struct board* const b,
        struct vec2 p,
        enum color c)
{
        assert(t != NULL);
        assert(b != NULL);

        if (is(t, p))
                *board_at(b, vec2_add_v(p, t->o)) = cell(c);
}

static inline bool in_bounds(
        const struct tet* const t, const struct board* const b, struct vec2 p)
{
        const struct vec2 dpos = vec2_add_v(p, t->o);
        return !(is(t, p) && (!board_in(b, dpos) || board_is(b, dpos)));
}

static inline bool in_rows(
        const struct tet* const t, const struct board* const b, struct vec2 p)
{
        const struct vec2 dpos = vec2_add_v(p, t->o);
        return !(is(t, p) && (dpos.row < 2));
}

static bool can_move(const struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        if (!in_bounds(t, b, p))
                                return false;

        return true;
}

static void move_by_dir(struct tet* const t, int d)
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
                        fill_with(t, b, p, t->c);
}

void tet_erase(const struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        fill_with(t, b, p, color_none);
}

bool tet_fellout(const struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        if (!in_rows(t, b, p))
                                return true;

        return false;
}

bool tet_move(struct tet* const t, struct board* const b, enum dir d)
{
        assert(t != NULL);
        assert(b != NULL);

        tet_erase(t, b);
        move_by_dir(t, d);

        const bool can = can_move(t, b);
        if (!can)
                move_by_dir(t, ~d);

        tet_draw(t, b);

        return can;
}

void tet_rotate(struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        tet_erase(t, b);

        t->rot = (t->rot + 1) % 4;
        if (!can_move(t, b))
                t->rot = (t->rot - 1) % 4;

        tet_draw(t, b);
}

void tet_drop(struct tet* const t, struct board* const b)
{
        assert(t != NULL);
        assert(b != NULL);

        while (tet_move(t, b, dir_down))
                ;
}

struct tet tet_spawn(const struct board* const b)
{
        return (struct tet) {
                .tp = get_rn(tet_tp_i, tet_tp_z),
                .rot = rot_none,
                .c = get_rn(color_cyan, color_red),
                .o = {
                        .row = 1,
                        .col = getmaxx(b->w) / 2 - 4,
                },
        };
}

/**** END OF FILE ****/
