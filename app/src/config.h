/*
 * @file   config.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CONFIG_H_INCLUDED_
#define CONFIG_H_INCLUDED_

#include "win.h"

static const struct win board = {
    .pos = {
        .row = 0,
        .col = 0,
    },
    .dim = {
        .row = 25,
        .col = 25
    },
    .border = true,
};

#endif /* CONFIG_H_INCLUDED_ */

/**** END OF FILE ****/
