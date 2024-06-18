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

static void win_fill_cell(struct win* const w, struct vec2 p)
{
        assert(w != NULL);
        assert(w->grid != NULL);

        const enum color c = win_atc(w, p);
        if (c != color_none) {
                wattron(w->p, COLOR_PAIR(c));
                mvwaddch(w->p, p.row, p.col, BLOCK);
                wattroff(w->p, COLOR_PAIR(c));
        } else {
                mvwaddch(w->p, p.row, p.col, EMPTY);
        }
}

static void win_fill(struct win* const w)
{
        assert(w != NULL);
        assert(w->p != NULL);
        assert(w->grid != NULL);

        for (u32 row = 1; row < w->dim.row - 1; row++)
                for (u32 col = 1; col < w->dim.col - 1; col++)
                        win_fill_cell(w, (struct vec2) { row, col });
}

int win_create(struct win* const w, bool use_grid)
{
        assert(w != NULL);

        w->p = newwin(w->dim.row, w->dim.col, w->pos.row, w->pos.col);
        if (w->p == NULL) {
                return ERR;
        }

        int error = 0;
        if (w->border) {
                error = box(w->p, 0, 0);
                if (error == ERR) {
                        return ERR;
                }
        }

        error = wrefresh(w->p);
        if (error == ERR) {
                return ERR;
        }

        if (use_grid) {
                w->grid = calloc(w->dim.row * w->dim.col, sizeof(enum color));
                if (w->grid == NULL) {
                        return ERR;
                }
        } else {
                w->grid = NULL;
        }

        return error;
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

void win_put(struct win* const w, u32 ch, struct vec2 p, enum color c)
{
        assert(w != NULL);
        assert(w->p != NULL);

        wattron(w->p, COLOR_PAIR(c));
        mvwaddch(w->p, p.row, p.col, ch);
        wattroff(w->p, COLOR_PAIR(c));
}

void win_destroy(struct win* const w)
{
        assert(w != NULL);
        assert(w->p != NULL);

        if (w->grid != NULL) {
                free(w->grid);
        }

        wrefresh(w->p);
        delwin(w->p);
}

void win_refresh(struct win* const w)
{
        assert(w != NULL);

        if (w->grid != NULL) {
                win_fill(w);
        }

        wrefresh(w->p);
}

/**** END OF FILE ****/
