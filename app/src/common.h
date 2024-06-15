/*
 * @file   common.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdint.h>

#define ARR_SIZE(arr) sizeof(arr) / sizeof(arr[0])

#define BLOCK ' ' | A_REVERSE
#define EMPTY ' '

typedef uint8_t u8;
typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;

enum color {
        color_none,
        color_cyan,
        color_yellow,
        color_blue,
        color_magenta,
        color_green,
        color_red,
};

enum dir {
        dir_down,
        dir_up,
        dir_right,
        dir_left,
};

struct vec2 {
        u32 row;
        u32 col;
};

#endif /* COMMON_H_INCLUDED */

/**** END OF FILE ****/
