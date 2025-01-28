/*
 * @file   game.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "game.h"

#include <assert.h>

#include "config.h"
#include "ncge/ncge.h"
#include "ncge/types.h"
#include "tet.h"

#define child_of(base) (struct game*)(base)

static void lower_down(struct board* const b, i32 row)
{
	assert(b != NULL);

	const struct vec2 max  = board_max(b);
	const struct vec2 down = { -1, 0 };

	struct vec2 p = { row, 1 };
	for (; p.row > 2; p.row--)
		for (p.col = 1; p.col < max.col; p.col++)
			board_at(b, p) = board_at(b, vec2_add_v(p, down));
}

static u8 erase_rows(struct board* const b)
{
	assert(b != NULL);

	struct vec2 p         = { 1, 1 };
	const struct vec2 max = board_max(b);

	u8 num_erased = 0;
	for (; p.row < max.row; p.row++) {
		for (p.col = 1; (p.col < max.col) && board_is(b, p); p.col++)
			continue;

		if (p.col == max.col) {
			lower_down(b, p.row);
			num_erased++;
		}
	}

	return num_erased;
}

static void swap_tets(struct ncge* const ctx)
{
	assert(ctx != NULL);

	struct game* const g = child_of(ctx);

	WINDOW* w = ncge_get_win(ctx, WIN_MAIN);

	g->curr       = g->next;
	g->curr.o.col = getmaxx(w) / 2 - 2;

	struct board* b = ncge_get_board(ctx, BOARD_NEXT);

	tet_erase(&g->next, b);
	g->next = tet_spawn(b);
	tet_draw(&g->next, b);
}

static inline int put_title(WINDOW* const w)
{
	return mvwprintw(w, 0, win_configs[WIN_MAIN].dim.col / 2 - 4, "ctetris");
}

static inline int put_score(WINDOW* const w, u32 score)
{
	return mvwprintw(
		w,
		win_configs[WIN_MAIN].dim.row - 1,
		win_configs[WIN_MAIN].dim.col / 2 - 4,
		"score: %d",
		score);
}

static inline int put_info(struct ncge* const ctx)
{
	assert(ctx != NULL);

	WINDOW* w = ncge_get_win(ctx, WIN_CTRL);
	int err   = mvwprintw(w, 0, 0, "%s", ctrl_txt);
	if (err < 0)
		return err;

	w   = ncge_get_win(ctx, WIN_NEXT);
	err = mvwprintw(
		w,
		win_configs[WIN_NEXT].dim.row - 1,
		win_configs[WIN_NEXT].dim.col / 2 - 2,
		"next");
	if (err < 0)
		return err;

	return err;
}

static inline int put_end_msg(WINDOW* const w, u32 score)
{
	return mvwprintw(w, 10, 0, "Final score: %d\nTry again? [y/N]", score);
}

static void init_tets(struct ncge* const ctx)
{
	assert(ctx != NULL);

	struct game* const g = child_of(ctx);
	struct board* b      = ncge_get_board(ctx, BOARD_GAME);

	board_clear(b);
	g->curr = tet_spawn(b);

	b = ncge_get_board(ctx, BOARD_NEXT);

	board_clear(b);
	g->next = tet_spawn(b);
	tet_draw(&g->next, b);

	g->score = 0;
	g->fell  = true;
}

static int on_lose(struct ncge* const ctx)
{
	assert(ctx != NULL);

	struct game* const g = child_of(ctx);
	WINDOW* const w      = ncge_get_win(ctx, WIN_CTRL);

	int err = put_end_msg(w, g->score);
	if (err < 0)
		return err;

	switch (getch()) {
	case 'y':
		ctx->state = STATE_INIT;
		break;
	case '\n':
	case 'n':
		ctx->state = STATE_EXIT;
		break;
	default:
		break;
	}

	return 0;
}

static int on_leave(struct ncge* const ctx)
{
	UNUSED(ctx);

	return NCGE_EXIT;
}

static int on_play(struct ncge* const ctx)
{
	assert(ctx != NULL);

	struct game* const g  = child_of(ctx);
	struct board* const b = ncge_get_board(ctx, BOARD_GAME);

	bool moved = true;

	switch (getch()) {
	case 's':
	case 'j':
	case KEY_DOWN:
		moved = tet_move(&g->curr, b, DIR_DOWN);
		break;
	case 'a':
	case 'h':
	case KEY_LEFT:
		tet_move(&g->curr, b, DIR_LEFT);
		break;
	case 'd':
	case 'l':
	case KEY_RIGHT:
		tet_move(&g->curr, b, DIR_RIGHT);
		break;
	case 'w':
	case 'k':
	case KEY_UP:
		tet_rotate(&g->curr, b);
		break;
	case ' ':
		tet_drop(&g->curr, b);
		moved = false;
		break;
	case '+':
		ctx->delay_step += DELAY_INC;
		break;
	case '-':
		ctx->delay_step -= DELAY_INC;
		break;
	case 'q':
	case KEY_ESC:
		ctx->state = STATE_EXIT;
		break;
	default:
		break;
	}

	g->fell &= moved;

	if (!g->fell) {
		if (tet_fellout(&g->curr)) {
			ctx->state = STATE_LOSE;
			return 0;
		}

		swap_tets(ctx);
		g->score += erase_rows(b);

		int err = put_score(b->w, g->score);
		if (err < 0)
			return err;
	}

	g->fell = true;

	return 0;
}

static int on_init(struct ncge* const ctx)
{
	assert(ctx != NULL);

	struct game* const g = child_of(ctx);
	WINDOW* w            = ncge_get_win(ctx, WIN_MAIN);

	int err = put_score(w, g->score);
	if (err < 0)
		return err;

	err = put_title(w);
	if (err < 0)
		return err;

	w = ncge_get_win(ctx, WIN_CTRL);

	wclear(w);

	err = put_info(ctx);
	if (err < 0)
		return err;

	init_tets(ctx);

	ctx->state = STATE_PLAY;

	return 0;
}

static int on_frame(struct ncge* const ctx)
{
	assert(ctx != NULL);

	struct game* const g  = child_of(ctx);
	struct board* const b = ncge_get_board(ctx, BOARD_GAME);

	g->fell = tet_move(&g->curr, b, DIR_DOWN);

	return 0;
}

static const struct ncge_ops ops = {
	.on_frame = on_frame,
	.state_ops = {
		[STATE_INIT] = on_init,
		[STATE_PLAY] = on_play,
		[STATE_LOSE] = on_lose,
		[STATE_EXIT] = on_leave,
	},
};

int game_run(struct game* const g)
{
	assert(g != NULL);

	struct ncge* const ctx = (struct ncge*)g;

	int err = ncge_init(ctx, &ops, 0);
	if (err < 0)
		return err;

	for (u8 i = 0; i < NUM_WINS; i++) {
		err = ncge_create_win(ctx, &win_configs[i]);
		if (err < 0)
			goto ncge_free;
	}

	for (u8 i = 0; i < NUM_BOARDS; i++) {
		err = ncge_create_board(ctx, i);
		if (err < 0)
			goto ncge_free;
	}

	return ncge_run(ctx);

ncge_free:
	ncge_destroy(&g->base);

	return err;
}

/**** END OF FILE ****/
