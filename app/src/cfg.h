/*
 * @file   cfg.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CFG_H_INCLUDED_
#define CFG_H_INCLUDED_

#include "common.h"

#define DELAY_INIT INT16_MAX
#define DELAY_STEP 1000
#define KEY_ESC    27

static const struct vec2 wgame_dim = { 50, 50 };
static const struct vec2 wgame_pos = { 0, 0 };

static const struct vec2 wscore_dim = { 4, 20 };
static const struct vec2 wscore_pos = { 1, wgame_dim.col + 1 };

static const struct vec2 wnext_dim = { 8, 10 };
static const struct vec2 wnext_pos = { wscore_dim.row - 1, wgame_dim.col + 4 };

static const struct vec2 wctrl_dim = { 10, 25 };
static const struct vec2 wctrl_pos = { wnext_dim.row + 2, wgame_dim.col + 1 };

static const struct vec2 wlogs_dim = { 10, 50 };
static const struct vec2 wlogs_pos = { wgame_dim.row, 0 };

static const char* ctrl_txt =
        "a/left/h: move left\n"
        "d/right/l: move right\n"
        "w/up/j: rotate\n"
        "space: hard drop";

#endif /* CFG_H_INCLUDED_ */

/**** END OF FILE ****/
