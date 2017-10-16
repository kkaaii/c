/******************************************************************************
** File Name: core.c
** Author:
** Creation Time: Mon Oct 16 06:54:52 2017
*/
#include "config.h"
#include "core.h"

static unsigned cells[ROWS][COLS];

static unsigned _ctz(unsigned x)
{
	return __builtin_ctz(x);
}

static int is_set(unsigned row, unsigned col)
{
	return 0 != (cells[row][col] & BIT(0));
}

static void _clr(unsigned row, unsigned col, unsigned num)
{
	if (!is_set(row, col))
		cells[row][col] &= ~BIT(num);
}

static void _set(unsigned row, unsigned col, unsigned num)
{
	unsigned cell = cells[row][col];
	cells[row][col] = (cell & ~ALL) | BIT(num) | BIT(0);
}

static void row_update(unsigned row, unsigned num)
{
	unsigned col;

	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			_clr(row, col, num);
		}
	}
}

static void col_update(unsigned col, unsigned num)
{
	unsigned row;

	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			_clr(row, col, num);
		}
	}
}

static void grp_update(unsigned grp, unsigned num)
{
	unsigned row, col;

	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			if (grp == get_grp(row, col)) {
				_clr(row, col, num);
			}
		}
	}
}

unsigned get_grp(unsigned row, unsigned col)
{
	return cells[row][col] >> (N + 1);
}

unsigned get_num(unsigned row, unsigned col)
{
	return is_set(row, col) ? _ctz(cells[row][col] & ALL) : 0;
}

void set_grp(unsigned row, unsigned col, unsigned grp)
{
	cells[row][col] |= grp << (N + 1);
}

void set_num(unsigned row, unsigned col, unsigned num)
{
	unsigned grp = get_grp(row, col);

	_set(row, col, num);

	row_update(row, num);
	col_update(col, num);
	grp_update(grp, num);
}

void init(void)
{
	unsigned row, col;

	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			cells[row][col] = ALL;
		}
	}
}

