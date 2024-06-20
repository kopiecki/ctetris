/*
 * @file   tetris.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TETRIS_H_INCLUDED_
#define TETRIS_H_INCLUDED_

int tetris_init(int argc, char* argv[]);
void tetris_destroy(void);

int tetris_run(void);

#endif /* TETRIS_H_INCLUDED_ */

/**** END OF FILE ****/
