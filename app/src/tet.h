/*
 * @file   tet.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TET_H_INCLUDED
#define TET_H_INCLUDED

#include "vec2.h"

enum tet_tp {
        tet_tp_i,
        tet_tp_o,
        tet_tp_l,
        tet_tp_j,
        tet_tp_t,
        tet_tp_s,
        tet_tp_z,
        tet_tp_end,
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
        struct vec2 pos;
};



#endif /* TET_H_INCLUDED */

/**** END OF FILE ****/
