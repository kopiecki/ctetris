/*
 * @file   game.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef GAME_H_INCLUDED_
#define GAME_H_INCLUDED_

#include "ncge/ncge.h"
#include "tet.h"

/**
 * struct game - Tetris game representation
 * @base:  Pointer to base class. In this case it is `ncge` layer.
 * @curr:  Current tetromino on game board.
 * @next:  Next tetromino to be displayed on game board.
 * @fell:  Flag indicating that tetromino successfully fell on the board.
 * @score: Game score.
 */
struct game {
	struct ncge base;
	struct tet curr;
	struct tet next;
	bool fell;
	u32 score;
};

int game_run(struct game* const g);

#endif /* GAME_H_INCLUDED_ */

/**** END OF FILE ****/
