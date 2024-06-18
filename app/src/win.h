/*
 * @file   win.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

#include <stdbool.h>

#include "common.h"

struct win {
        void* p;
        enum color* grid;
        struct vec2 pos;
        struct vec2 dim;
        const bool border;
};

int win_create(struct win* const w);
void win_destroy(struct win* const w);

void win_print(struct win* const w, const char* fmt, ...);
void win_put(struct win* const w, struct vec2 p, struct cell c);

int win_refresh(struct win* const w);

#endif /* WIN_H_INCLUDED */

/**** END OF FILE ****/
