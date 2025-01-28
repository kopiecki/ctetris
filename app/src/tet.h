/**
 * @file   tet.h
 * @brief  Tetromino representation
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#ifndef TET_H_INCLUDED_
#define TET_H_INCLUDED_

#include "ncge/board.h"

/**
 * @brief Tetromino type
 */
enum tet_tp {
	TET_I,
	TET_O,
	TET_L,
	TET_J,
	TET_T,
	TET_S,
	TET_Z,
};

/**
 * @brief Tetromino rotation
 */
enum rot {
	ROT_NONE,
	ROT_LEFT,
	ROT_RIGHT,
	ROT_REV,
};

/**
 * @brief Tetromino representation
 */
struct tet {
	enum tet_tp tp; /**< Tetromino type */
	enum rot rot;   /**< Current rotation*/
	enum color c;   /**< Tetromino color*/
	struct vec2 o;  /**< Tetromino origin position*/
	chtype ch;      /**< Character type, representin single tetromino block*/
};

/**
 * @brief Draw tetromino on the board
 *
 * @param[in] t Instance pointer
 * @param[in] b Board at which we are drawing
 */
void tet_draw(const struct tet* const t, struct board* const b);

/**
 * @brief Erase tetromino from the board
 *
 * @param[in] t Instance pointer
 * @param[in] b Board at which we are erasing
 */
void tet_erase(const struct tet* const t, struct board* const b);

/**
 * @brief Check if tetromino go out of row range (Y direction)
 *
 * @param[in] t Instance pointer
 * @param[in] b Board at which tetromino is drawn
 * @return true
 * @return false
 */
bool tet_fellout(const struct tet* const t);

/**
 * @brief Move tetromino in certain direction
 *
 * @param[in] t  Instance pointer
 * @param[in] b  Board at which tetromino is drawn
 * @param[in] d  Direction at which tetromino will be moved
 * @return true  Succesfully moved
 * @return false Can't move
 */
bool tet_move(struct tet* const t, struct board* const b, enum dir d);

/**
 * @brief Rotate tetromino
 *
 * @param[in] t Instance pointer
 * @param[in] b Board at which tetromino is drawn
 */
void tet_rotate(struct tet* const t, struct board* const b);

/**
 * @brief Hard drop tetromino
 *
 * @param[in] t Instance pointer
 * @param[in] b Board ath which tetromino is drawn
 */
void tet_drop(struct tet* const t, struct board* const b);

/**
 * @brief Spawn random tetromino with random color at the middle of the board
 *
 * @param[in] b       Board at which tetromino will be spawned
 * @return struct tet Spawned tetromino
 */
struct tet tet_spawn(const struct board* const b);

#endif /* TET_H_INCLUDED_ */

/**** END OF FILE ****/
