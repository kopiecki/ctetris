/**
 * @file   game.h
 * @brief  Tetris game
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#ifndef GAME_H_INCLUDED_
#define GAME_H_INCLUDED_

#include "ncge/ncge.h"
#include "tet.h"

/**
 * @brief Tetris game representation
 * @base:  Pointer to base class. In this case it is `ncge` layer.
 * @curr:  Current tetromino on game board.
 * @next:  Next tetromino to be displayed on game board.
 * @fell:  Flag indicating that tetromino successfully fell on the board.
 * @score: Game score.
 */
struct game {
	struct ncge base; /**< Base class, ncge backend*/
	struct tet curr;  /**< Current tetromino on game board*/
	struct tet next;  /**< Next tetromino to be displayed*/
	bool fell;        /**< Tetromino successfully fell on the board indicator*/
	u32 score;        /**< Current score*/
};

/**
 * @brief Run game
 *
 * @param g    Instance pointer
 * @return int 0 on success, negative errno otherwise
 */
int game_run(struct game* const g);

/**
 * @brief Destroy game
 *
 * @param g    Instance pointer
 */
void game_destroy(struct game* const g);

#endif /* GAME_H_INCLUDED_ */

/**** END OF FILE ****/
