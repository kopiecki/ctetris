/*
 * @file   win.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "win.h"

#include <assert.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int win_create(struct win* const w)
{
        assert(w != NULL);

        w->p = newwin(w->dim.row, w->dim.col, w->pos.row, w->pos.col);
        if (w->p == NULL)
                return ERR;

        int error = 0;
        if (w->border) {
                error = box(w->p, 0, 0);
                if (error == ERR)
                        return ERR;
        }

        return win_refresh(w);
}

void win_print(struct win* const w, const char* fmt, ...)
{
        assert(w != NULL);
        assert(w->p != NULL);

        wclear(w->p);

        va_list args;
        va_start(args, fmt);
        vw_printw(w->p, fmt, args);
        va_end(args);
}

void win_put(struct win* const w, struct vec2 p, struct cell c)
{
        assert(w != NULL);
        assert(w->p != NULL);

        wattron(w->p, COLOR_PAIR(c.c));
        mvwaddch(w->p, p.row, p.col, c.ch);
        wattroff(w->p, COLOR_PAIR(c.c));
}

void win_destroy(struct win* const w)
{
        assert(w != NULL);
        assert(w->p != NULL);

        delwin(w->p);
}

int win_refresh(struct win* const w)
{
        assert(w != NULL);

        return wrefresh(w->p);
}

/**** END OF FILE ****/
