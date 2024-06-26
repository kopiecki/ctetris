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
#define DELAY_STEP 600
#define KEY_ESC    27

enum win_tp {
        wgame,
        wnext,
        wctrl,
        wlogs,
        num_wins,
};

enum board_tp {
        bgame,
        bnext,
        num_boards,
};

static const struct vec2 dims[] = {
        [wgame] = { 30, 30 },
        [wnext] = { 8, 10 },
        [wctrl] = { 10, 25 },
        [wlogs] = { 15, 28 },
};

static const struct vec2 poses[] = {
        [wgame] = { 0, 0 },
        [wnext] = { 0, dims[wgame].col + 3 },
        [wctrl] = { dims[wnext].row - 1, dims[wgame].col + 1 },
        [wlogs] = { dims[wctrl].row + 5, dims[wgame].col + 1 },
};

static const char* ctrl_txt =
        "a/left/h: move left\n"
        "d/right/l: move right\n"
        "w/up/j: rotate\n"
        "space: hard drop";

#endif /* CFG_H_INCLUDED_ */

/**** END OF FILE ****/
