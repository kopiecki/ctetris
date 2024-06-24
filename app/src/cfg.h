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

enum win_tp {
        wgame,
        wnext,
        wscore,
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
        [wgame] = { 50, 50 }, [wnext] = { 8, 10 },  [wscore] = { 4, 20 },
        [wctrl] = { 10, 25 }, [wlogs] = { 10, 50 },
};

static const struct vec2 poses[] = {
        [wgame]  = { 0, 0 },
        [wscore] = { 1, dims[wgame].col + 1 },
        [wnext]  = { dims[wscore].row - 1, dims[wgame].col + 4 },
        [wctrl]  = { dims[wnext].row + 2, dims[wgame].col + 1 },
        [wlogs]  = { dims[wgame].row, 0 },
};

static const char* ctrl_txt =
        "a/left/h: move left\n"
        "d/right/l: move right\n"
        "w/up/j: rotate\n"
        "space: hard drop";

#endif /* CFG_H_INCLUDED_ */

/**** END OF FILE ****/
