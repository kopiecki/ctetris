/*
 * @file   win.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "win.h"

#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>

#include "common.h"

static void win_fill_cell(struct win* const w, u32 row, u32 col)
{
        assert(w != NULL);
        assert(w->grid != NULL);

        const enum color c = *win_at(w, row, col);
        if (c) {
                wattron(w->p, COLOR_PAIR(c));
                mvwaddch(w->p, row, col, BLOCK);
                wattroff(w->p, COLOR_PAIR(c));
        } else {
                mvwaddch(w->p, row, col, EMPTY);
        }
}

static void win_fill(struct win* const w)
{
        assert(w != NULL);
        assert(w->p != NULL);
        assert(w->grid != NULL);

        for (u32 row = 1; row < w->dim.row - 1; row++)
                for (u32 col = 1; col < w->dim.col - 1; col++)
                        win_fill_cell(w, row, col);
}

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

        error = wrefresh(w->p);
        if (error == ERR)
                return ERR;

        w->grid = calloc(w->dim.row * w->dim.col, sizeof(enum color));
        if (w->grid == NULL)
                return ERR;

        return error;
}

void win_destroy(struct win* const w)
{
        assert(w != NULL);
        assert(w->p != NULL);
        assert(w->grid != NULL);

        free(w->grid);
        wrefresh(w->p);
        delwin(w->p);
}

void win_refresh(struct win* const w)
{
        assert(w != NULL);
        assert(w->p != NULL);

        win_fill(w);
        wrefresh(w->p);
}

/**** END OF FILE ****/
