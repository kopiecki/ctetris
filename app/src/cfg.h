/*
 * @file   cfg.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CFG_H_INCLUDED_
#define CFG_H_INCLUDED_

#include <stdbool.h>

#include "common.h"

#define DELAY_INIT INT16_MAX
#define DELAY_STEP (1 << 9)
#define DELAY_INC  (1 << 6)
#define KEY_ESC    27

enum win_tp {
        wgame,
        wnext,
        wctrl,
        num_wins,
};

enum board_tp {
        bgame,
        bnext,
        num_boards,
};

static const struct vec2 dims[] = {
        [wgame] = { 30, 30 },
        [wnext] = { 6, 10 },
        [wctrl] = { 10, 25 },
};

static const struct vec2 poses[] = {
        [wgame] = { 0, 0 },
        [wnext] = { 0, dims[wgame].col + 6 },
        [wctrl] = { dims[wnext].row + 1, dims[wgame].col + 1 },
};

static const bool borders[] = {
        [wgame] = true,
        [wnext] = true,
        [wctrl] = false,
};

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
