/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <ncurses.h>

#include "tetris.h"

int main(int argc, char* argv[])
{
        int err = tetris_init(argc, argv);
        if (err == ERR)
                return ERR;

        return tetris_run();
}

/**** END OF FILE ****/
