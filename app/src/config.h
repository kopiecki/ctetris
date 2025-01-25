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

#include "ncge/ncge.h"

#define DELAY_INIT INT16_MAX
#define DELAY_STEP (1 << 9)
#define DELAY_INC  (1 << 6)
#define KEY_ESC    27

/**
 * @brief Type of windows that ctetris game consists of
 */
enum win_tp {
	win_tp_main = 0,
	win_tp_next,
	win_tp_ctrl,
	num_wins,
};

/**
 * @brief Type of boards that ctetris game consits of
 *
 */
enum board_tp {
	board_tp_game = 0,
	board_tp_next,
	num_boards,
};

/**
 * @brief Windows configuration
 */
static const struct win_cfg win_configs[] = {
	[win_tp_main] = {
		.dim = {
			.row = 30,
			.col = 30
		},
		.pos = {
			.row = 0,
			.col = 0,
		},
		.border = true
	},
	[win_tp_next] = {
		.dim = {
			.row = 6,
			.col = 10
		},
		.pos = {
			.row = 0,
			.col = 30 + 6,
		},
		.border = true
	},
	[win_tp_ctrl] = {
		.dim = {
			.row = 20,
			.col = 25
		},
		.pos = {
			.row = 6 + 1,
			.col = 30 + 1,
		},
		.border = false,
	}
};

/**
 * @brief Game controls information text
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
