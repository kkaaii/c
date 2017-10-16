/******************************************************************************
** File Name: print.c
** Author:
** Creation Time: Mon Oct 16 07:31:49 2017
*/
#include <stdio.h>
#include "config.h"
#include "core.h"
#include "print.h"
#include "colors.h"

static void print_cell(unsigned row, unsigned col)
{
	unsigned num = get_num(row, col);
	unsigned grp = get_grp(row, col);

	printf("%s%c", get_color(grp), num ? num + '0' : ' ');
}

void print(void)
{
	unsigned row, col;

	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			print_cell(row, col);
		}
		puts(RESET);
	}
}

