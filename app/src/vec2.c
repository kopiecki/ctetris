/*
 * @file   vec2.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "vec2.h"

struct vec2 vec2_add_v(const struct vec2* const rhs, const struct vec2* const lhs)
{
        const struct vec2 r = {
                .x = rhs->x + rhs->x,
                .y = rhs->y + rhs->y,
        };

        return r;
}

struct vec2 vec2_add_s(const struct vec2* const rhs, u32 s)
{
        const struct vec2 r = {
                .x = rhs->x + s,
                .y = rhs->y + s,
        };

        return r;
}

struct vec2 vec2_sub_v(const struct vec2* const rhs, const struct vec2* const lhs)
{
        const struct vec2 r = {
                .x = rhs->x - rhs->x,
                .y = rhs->y - rhs->y,
        };

        return r;
}

struct vec2 vec2_sub_s(const struct vec2* const rhs, u32 s)
{
        const struct vec2 r = {
                .x = rhs->x - s,
                .y = rhs->y - s,
        };

        return r;
}

struct vec2 vec2_mult_v(const struct vec2* const rhs, const struct vec2* const lhs)
{
        const struct vec2 r = {
                .x = rhs->x * rhs->x,
                .y = rhs->y * rhs->y,
        };

        return r;
}

struct vec2 vec2_mult_s(const struct vec2* const rhs, u32 s)
{
        const struct vec2 r = {
                .x = rhs->x * s,
                .y = rhs->y * s,
        };

        return r;
}

struct vec2 vec2_div_v(const struct vec2* const rhs, const struct vec2* const lhs)
{
        const struct vec2 r = {
                .x = rhs->x / rhs->x,
                .y = rhs->y / rhs->y,
        };

        return r;
}

struct vec2 vec2_div_s(const struct vec2* const rhs, u32 s)
{
        const struct vec2 r = {
                .x = rhs->x / s,
                .y = rhs->y / s,
        };

        return r;
}

/**** END OF FILE ****/
