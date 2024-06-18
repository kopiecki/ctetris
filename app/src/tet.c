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

static void tet_fill_with(
    const struct tet* const t, struct win* const w, struct vec2 p, enum color c)
{
        assert(t != NULL);
        assert(w != NULL);

        if (tet_is(t, p))
                *win_at(w, vec2_add_v(p, t->o)) = c;
}

static bool tet_in_bounds(
    const struct tet* const t, const struct win* const w, struct vec2 p)
{
        assert(t != NULL);
        assert(w != NULL);

        const struct vec2 dpos = vec2_add_v(p, t->o);

        if (tet_is(t, p))
                if (!win_in_bounds(w, dpos) || win_atc(w, dpos))
                        return false;

        return true;
}

static bool tet_can_move(const struct tet* const t, struct win* const w)
{
        assert(t != NULL);
        assert(w != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        if (!tet_in_bounds(t, w, p))
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

void tet_draw(const struct tet* const t, struct win* const w)
{
        assert(t != NULL);
        assert(w != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        tet_fill_with(t, w, p, t->c);
}

void tet_erase(const struct tet* const t, struct win* const w)
{
        assert(t != NULL);
        assert(w != NULL);

        struct vec2 p = { 0, 0 };
        for (; p.row < NUM_ROWS; p.row++)
                for (p.col = 0; p.col < NUM_COLS; p.col++)
                        tet_fill_with(t, w, p, color_none);
}

bool tet_move(struct tet* const t, struct win* const w, enum dir d)
{
        assert(t != NULL);
        assert(w != NULL);

        tet_erase(t, w);
        tet_move_by_dir(t, d);

        const bool can_move = tet_can_move(t, w);
        if (!can_move)
                tet_move_by_dir(t, ~d);

        tet_draw(t, w);

        return can_move;
}

void tet_rotate(struct tet* const t, struct win* const w)
{
        assert(t != NULL);
        assert(w != NULL);

        tet_erase(t, w);

        t->rot = (t->rot + 1) % 4;
        if (!tet_can_move(t, w))
                t->rot = (t->rot - 1) % 4;

        tet_draw(t, w);
}

struct tet tet_spawn(const struct win* const w)
{
        return (struct tet) {
                .tp = get_rn(tet_tp_i, tet_tp_z),
                .rot = rot_none,
                .c = get_rn(color_cyan, color_red),
                .o = {
                        .row = 1,
                        .col = w->dim.col / 2 - 4,
                },
        };
}

/**** END OF FILE ****/
