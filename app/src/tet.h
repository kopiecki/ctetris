/*
 * @file   tet.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TET_H_INCLUDED
#define TET_H_INCLUDED

#include "board.h"

/**
 * @brief Tetromino type
 */
enum tet_tp {
        tet_tp_i,
        tet_tp_o,
        tet_tp_l,
        tet_tp_j,
        tet_tp_t,
        tet_tp_s,
        tet_tp_z,
};

/**
 * @brief Tetromino rotation
 */
enum rot {
        rot_none,
        rot_left,
        rot_right,
        rot_rev,
};

/**
 * @brief Tetromino representation
 */
struct tet {
        enum tet_tp tp;
        enum rot rot;
        enum color c;
        struct vec2 o;
        chtype ch;
};

/**
 * @brief Draw tetromino on the board
 *
 * @param t Instance pointer
 * @param b Board at which we are drawing
 */
void tet_draw(const struct tet* const t, struct board* const b);

/**
 * @brief Erase tetromino from the board
 *
 * @param t Instance pointer
 * @param b Board at which we are erasing
 */
void tet_erase(const struct tet* const t, struct board* const b);

/**
 * @brief Check if tetromino go out of row range (Y direction)
 *
 * @param t Instance pointer
 * @param b Board at which tetromino is drawn
 * @return true
 * @return false
 */
bool tet_fellout(const struct tet* const t);

/**
 * @brief Move tetromino in certain direction
 *
 * @param t             Instance pointer
 * @param b             Board at which tetromino is drawn
 * @param d             Direction at which tetromino will be moved
 * @return true         Succesfully moved
 * @return false        Can't move
 */
bool tet_move(struct tet* const t, struct board* const b, enum dir d);

/**
 * @brief Rotate tetromino
 *
 * @param t Instance pointer
 * @param b Board at which tetromino is drawn
 */
void tet_rotate(struct tet* const t, struct board* const b);

/**
 * @brief Hard drop tetromino
 *
 * @param t Instance pointer
 * @param b Board ath which tetromino is drawn
 */
void tet_drop(struct tet* const t, struct board* const b);

/**
 * @brief Spawn random tetromino with random color at the middle of the board
 *
 * @param b           Board at which tetromino will be spawned
 * @return struct tet Spawned tetromino
 */
struct tet tet_spawn(const struct board* const b);

#endif /* TET_H_INCLUDED */

/**** END OF FILE ****/
