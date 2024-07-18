/*
 * @file   cfg.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CFG_H_INCLUDED_
#define CFG_H_INCLUDED_

#include <complex.h>
#include <stdbool.h>

#include "common.h"

#define DELAY_INIT INT16_MAX
#define DELAY_STEP (1 << 9)
#define DELAY_INC  (1 << 6)
#define KEY_ESC    27

/**
 * @brief Type of windows that ctetris game consists of
 */
enum win_tp {
	wgame,
	wnext,
	wctrl,
	wprompt,
	num_wins,
};

/**
 * @brief Type of boards that ctetris game consits of
 *
 */
enum board_tp {
	bgame,
	bnext,
	num_boards,
};

/**
 * @brief Window dimensions
 */
static const struct vec2 dims[] = {
	[wgame]   = { 30, 30 },
	[wnext]   = { 6, 10 },
	[wctrl]   = { 10, 25 },
	[wprompt] = { 10, 25 },
};

/**
 * @brief Window positions
 */
static const struct vec2 poses[] = {
	[wgame]   = { 0, 0 },
	[wnext]   = { 0, dims[wgame].col + 6 },
	[wctrl]   = { dims[wnext].row + 1, dims[wgame].col + 1 },
	[wprompt] = { dims[wnext].row + dims[wctrl].row + 1, dims[wgame].col + 1 }
};

/**
 * @brief Window borders indicators
 */
static const bool borders[] = {
	[wgame]   = true,
	[wnext]   = true,
	[wctrl]   = false,
	[wprompt] = false,
};

/**
 * @brief Controls text
 */
static const char* ctrl_txt =
	"controls\n"
	"---------------------\n"
	"a/left/h: move left\n"
	"d/right/l: move right\n"
	"w/up/j: rotate\n"
	"space: hard drop\n"
	"+/-: change speed\n"
	"q/ESC/ctrl+c: quit\n"
	"---------------------\n";

#endif /* CFG_H_INCLUDED_ */

/**** END OF FILE ****/
