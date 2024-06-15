/*
 * @file   tet.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "tet.h"

#include <curses.h>

#include "common.h"
#include "tet_srs_lut.h"
#include "win.h"

#define BLOCK ' ' | A_REVERSE
#define NONE  ' '

#define COL_STEP 2
#define ROW_STEP 1

static inline bool tet_is(const struct tet* const t, u8 row, u8 col)
{
        return tet_srs_lut[t->tp][t->rot][row][col];
}

static void tet_fill_with(
    const struct tet* const t, struct win* const w, u8 row, u8 col, enum color c)
{
        if (tet_is(t, row, col)) {
                *win_at(w, t->origin.row + row, t->origin.col + col) = c;
        }
}

struct tet tet_create(enum tet_tp tp, enum color c)
{
        const struct tet t = {
                .tp = tp,
                .rot = rot_none,
                .c = c,
                .origin = {
                        .row = 2,
                        .col = 2,
                },
        };

        return t;
}

void tet_draw(const struct tet* const t, struct win* const w)
{
        for (u8 row = 0; row < NUM_ROWS; row++) {
                for (u8 col = 0; col < NUM_COLS; col++) {
                        tet_fill_with(t, w, row, col, t->c);
                }
        }
}

void tet_erase(const struct tet* const t, struct win* const w)
{
        for (u8 row = 0; row < NUM_ROWS; row++) {
                for (u8 col = 0; col < NUM_COLS; col++) {
                        tet_fill_with(t, w, row, col, color_none);
                }
        }
}

void tet_rotate(struct tet* const t, struct win* const w)
{
        tet_erase(t, w);

        if (t->rot < rot_rev) {
                t->rot++;
        } else {
                t->rot = rot_none;
        }

        tet_draw(t, w);
}

static bool tet_in_bounds_row_col(
    const struct tet* const t, struct win* const w, u32 row, u32 col, bool check_cols)
{
        if (tet_is(t, row, col)) {
                const u32 drow = row + t->origin.row;
                const u32 dcol = col + t->origin.col;

                bool collides = *win_at(w, drow + 1, dcol);
                bool in_rows  = (drow < w->dim.row - 2 && drow - 1 > 0);
                bool in_cols  = (dcol < w->dim.col && dcol > 2);

                if (check_cols) {
                        return !collides && in_rows && in_cols;
                } else {
                        return !collides && in_rows;
                }
        }

        return true;
}

static bool tet_in_bounds(struct tet* const t, struct win* const w, bool check_cols)
{
        u8 drow, dcol;
        for (u8 row = 0; row < NUM_ROWS; row++) {
                for (u8 col = 0; col < NUM_COLS; col++) {
                        if (!tet_in_bounds_row_col(t, w, row, col, check_cols)) {
                                return false;
                        }
                }
        }

        return true;
}

static bool tet_can_move_left(struct tet* const t, struct win* const w)
{
        u8 drow, dcol;
        for (u8 row = 0; row < NUM_ROWS; row++) {
                for (u8 col = 0; col < NUM_COLS; col++) {
                        if (tet_is(t, row, col)) {
                                const u32 drow = row + t->origin.row;
                                const u32 dcol = col + t->origin.col;

                                bool collides = *win_at(w, drow + 1, dcol);
                                bool can_move = (dcol > 4);

                                return !collides && can_move;
                        }
                }
        }

        return true;
}

static bool tet_can_move_right(struct tet* const t, struct win* const w)
{
        u8 drow, dcol;
        for (u8 row = 0; row < NUM_ROWS; row++) {
                for (u8 col = 0; col < NUM_COLS; col++) {
                        if (tet_is(t, row, col)) {
                                const u32 drow = row + t->origin.row;
                                const u32 dcol = col + t->origin.col;

                                bool collides = *win_at(w, drow + 1, dcol);
                                bool can_move = (dcol <= w->dim.col);

                                return !collides && can_move;
                        }
                }
        }

        return true;
}

void tet_move_left(struct tet* const t, struct win* const w)
{
        tet_erase(t, w);

        if (tet_can_move_left(t, w)) {
                t->origin.col -= COL_STEP;
        }

        tet_draw(t, w);
}

void tet_move_right(struct tet* const t, struct win* const w)
{
        tet_erase(t, w);

        if (tet_can_move_right(t, w)) {
                t->origin.col += COL_STEP;
        }

        tet_draw(t, w);
}

bool tet_move_down(struct tet* const t, struct win* const w)
{
        tet_erase(t, w);

        bool moved = false;
        if (tet_in_bounds(t, w, false)) {
                t->origin.row += ROW_STEP;
                moved = true;
        }

        tet_draw(t, w);

        return moved;
}

/**** END OF FILE ****/
