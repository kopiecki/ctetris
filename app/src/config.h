/**
 * @file   config.h
 * @brief  Configuration file
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#ifndef CONFIG_H_INCLUDED_
#define CONFIG_H_INCLUDED_

#include "ncge/ncge.h"

#define DELAY_INIT INT16_MAX
#define DELAY_STEP (1 << 9)
#define DELAY_INC  (1 << 6)
#define KEY_ESC    27

/**
 * @brief Type of windows that ctetris game consists of
 */
enum win_tp {
	WIN_MAIN = 0,
	WIN_NEXT,
	WIN_CTRL,
	NUM_WINS,
};

/**
 * @brief Type of boards that ctetris game consits of
 *
 */
enum board_tp {
	BOARD_GAME = 0,
	BOARD_NEXT,
	NUM_BOARDS,
};

/**
 * @brief Windows configuration
 */
static const struct win_cfg win_configs[] = {
	[WIN_MAIN] = {
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
	[WIN_NEXT] = {
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
	[WIN_CTRL] = {
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

#endif /* CONFIG_H_INCLUDED_ */

/**** END OF FILE ****/
