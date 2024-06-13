/*
 * @file   vec2.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef VEC2_H_INCLUDED
#define VEC2_H_INCLUDED

#include "common.h"

struct vec2 {
        u32 x;
        u32 y;
};

struct vec2 vec2_add_v(const struct vec2* const rhs, const struct vec2* const lhs);
struct vec2 vec2_add_s(const struct vec2* const rhs, u32 s);

struct vec2 vec2_sub_v(const struct vec2* const rhs, const struct vec2* const lhs);
struct vec2 vec2_sub_s(const struct vec2* const rhs, u32 s);

struct vec2 vec2_mult_v(const struct vec2* const rhs, const struct vec2* const lhs);
struct vec2 vec2_mult_s(const struct vec2* const rhs, u32 s);

struct vec2 vec2_div_v(const struct vec2* const rhs, const struct vec2* const lhs);
struct vec2 vec2_div_s(const struct vec2* const rhs, u32 s);

#endif /* VEC2_H_INCLUDED */

/**** END OF FILE ****/
