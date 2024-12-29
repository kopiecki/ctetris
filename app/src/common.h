/*
 * @file   common.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

#include "ncge/types.h"

/**
 * @brief Get random intiger within (lower, upper) range
 *
 * @param lower         Lower range
 * @param upper         Upper range
 * @return int          Random integer
 */


/**
 * @brief Get maximal rows and columns of created window
 *
 * @param w             ncurses window pointer
 * @return struct vec2  Maximal rows and columns
 */
static inline struct vec2 win_max(const WINDOW* const w)
{
	return (struct vec2) {
		.row = getmaxy(w) - 1,
		.col = getmaxx(w) - 1,
	};
}

#endif /* COMMON_H_INCLUDED */

/**** END OF FILE ****/
