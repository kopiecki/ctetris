/**
 * @file   tet.c
 * @brief  Tetromino representation
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#include "tet.h"

#include <assert.h>
#include <curses.h>
#include <stdlib.h>

#include "ncge/board.h"
#include "ncge/types.h"
#include "tet_srs_lut.h"

#define COL_STEP 2
#define ROW_STEP 1

#define TET_INIT_COL 20
#define TET_INIT_ROW 1

inline static int get_rn(int lower, int upper)
{
	return (rand() % (upper - lower + 1)) + lower;
}

static inline bool is(const struct tet* const t, struct vec2 p)
{
	return tet_srs_lut[t->tp][t->rot][p.row][p.col];
}

static inline struct cell cell(enum color c)
{
	if (c == COL_NONE)
		return CELL_EMPTY;

	return cell_create(BLOCK, c);
}

static void fill_with(
	const struct tet* const t, struct board* const b, struct vec2 p, enum color c)
{
	assert(t != NULL);
	assert(b != NULL);

	if (is(t, p))
		board_at(b, vec2_add_v(p, t->o)) = cell(c);
}

static inline bool in_bounds(
	const struct tet* const t, const struct board* const b, struct vec2 p)
{
	const struct vec2 dpos = vec2_add_v(p, t->o);
	return !(is(t, p) && (!board_in(b, dpos) || board_is(b, dpos)));
}

static inline bool in_rows(const struct tet* const t, struct vec2 p)
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
	case DIR_DOWN:
		t->o.row += ROW_STEP;
		break;
	case DIR_RIGHT:
		t->o.col += COL_STEP;
		break;
	case DIR_LEFT:
		t->o.col -= COL_STEP;
		break;
	case ~DIR_DOWN:
		t->o.row -= ROW_STEP;
		break;
	case ~DIR_LEFT:
		t->o.col += COL_STEP;
		break;
	case ~DIR_RIGHT:
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
			fill_with(t, b, p, COL_NONE);
}

bool tet_fellout(const struct tet* const t)
{
	assert(t != NULL);

	struct vec2 p = { 0, 0 };
	for (; p.row < NUM_ROWS; p.row++)
		for (p.col = 0; p.col < NUM_COLS; p.col++)
			if (!in_rows(t, p))
				return true;

	return false;
}

bool tet_move(struct tet* const t, struct board* const b, enum dir d)
{
	assert(t != NULL);
	assert(b != NULL);

	tet_erase(t, b);
	move_by_dir(t, (i32)d);

	const bool can = can_move(t, b);
	if (!can)
		move_by_dir(t, (i32)~d);

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

	while (tet_move(t, b, DIR_DOWN))
		;
}

struct tet tet_spawn(const struct board* const b)
{
	return (struct tet) {
		.tp  = (enum tet_tp)get_rn(TET_I, TET_Z),
		.rot = ROT_NONE,
		.c   = (enum color)get_rn(COL_CYAN, COL_RED),
		.o   = { .row = 1, .col = getmaxx(b->w) / 2 - 4 },
	};
}

/**** END OF FILE ****/
