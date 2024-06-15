/*
 * @file   tet.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TET_H_INCLUDED
#define TET_H_INCLUDED

#include "win.h"

enum tet_tp {
        tet_tp_i,
        tet_tp_o,
        tet_tp_l,
        tet_tp_j,
        tet_tp_t,
        tet_tp_s,
        tet_tp_z,
};

enum rot {
        rot_none,
        rot_left,
        rot_right,
        rot_rev,
};

struct tet {
        enum tet_tp tp;
        enum rot rot;
        enum color c;
        struct vec2 origin;
};

struct tet tet_create(enum tet_tp tp, enum color c);

void tet_draw(const struct tet* const t, struct win* const w);
void tet_erase(const struct tet* const t, struct win* const w);

bool tet_move_down(struct tet* const t, struct win* const w);
void tet_move_left(struct tet* const t, struct win* const w);
void tet_move_right(struct tet* const t, struct win* const w);

void tet_rotate(struct tet* const t, struct win* const w);

#endif /* TET_H_INCLUDED */

/**** END OF FILE ****/
