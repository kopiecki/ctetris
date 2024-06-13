/*
 * @file   win.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "win.h"

WINDOW* win_create(const struct win_cfg* const cfg)
{
        WINDOW* win = newwin(cfg->dim.x, cfg->dim.y, cfg->pos.x, cfg->pos.y);
        if (cfg->border && win) {
                box(win, 0, 0);
                wrefresh(win);
        }

        return win;
}

void win_destroy(WINDOW* win)
{
        wrefresh(win);
        delwin(win);
}

/**** END OF FILE ****/
