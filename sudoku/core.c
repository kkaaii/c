/******************************************************************************
** File Name: core.c
** Author:
** Creation Time: Mon Oct 16 06:54:52 2017
*/
#include "config.h"
#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#define	DEBUG_MSG	printf

#define	DEBUG_ASSERT(x)	do {												\
	if (!(x)) {																\
		DEBUG_MSG("Assertion: \"%s\", %s:%d\n", #x, __FILE__, __LINE__);	\
		exit(-1);															\
	}																		\
} while (0)

static unsigned cells[ROWS][COLS];

static unsigned _ctz(unsigned x)
{
	return __builtin_ctz(x);
}

static int is_set(unsigned row, unsigned col)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);

	return 0 != (cells[row][col] & BIT(0));
}

static void _clr(unsigned row, unsigned col, unsigned num, ESetType t)
{
	unsigned cell;

	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);
	DEBUG_ASSERT(0 < num && num <= N);

	if (!is_set(row, col)) {
		cells[row][col] &= ~BIT(num);
		cell = cells[row][col] & ALL;
		DEBUG_ASSERT(0 != cell);

		if (0 == (cell & (cell - 1))) {
			set_num(row, col, _ctz(cell), t);
		}
	}
}

static void _set(unsigned row, unsigned col, unsigned num)
{
	unsigned cell;

	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);
	DEBUG_ASSERT(num - 1 < N);

	cell = cells[row][col];
	cells[row][col] = (cell & ~ALL) | BIT(num) | BIT(0);
}

static void row_update(unsigned row, unsigned num)
{
	unsigned col;

	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(num - 1 < N);

	for (col = 0; col < COLS; ++col) {
		_clr(row, col, num, eUniqueInRow);
	}
}

static void col_update(unsigned col, unsigned num)
{
	unsigned row;

	DEBUG_ASSERT(col < COLS);
	DEBUG_ASSERT(num - 1 < N);

	for (row = 0; row < ROWS; ++row) {
		_clr(row, col, num, eUniqueInCol);
	}
}

static void grp_update(unsigned grp, unsigned num)
{
	unsigned row, col;

	DEBUG_ASSERT(grp - 1 < GRPS);
	DEBUG_ASSERT(num - 1 < N);

	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			if (grp == get_grp(row, col)) {
				_clr(row, col, num, eUniqueInGrp);
			}
		}
	}
}

unsigned get_grp(unsigned row, unsigned col)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);

	return cells[row][col] >> (N + 1);
}

unsigned get_num(unsigned row, unsigned col)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);

	return is_set(row, col) ? _ctz(cells[row][col] & ALL) : 0;
}

void set_grp(unsigned row, unsigned col, unsigned grp)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);
	DEBUG_ASSERT(grp - 1 < GRPS);

	cells[row][col] |= grp << (N + 1);
}

void set_num(unsigned row, unsigned col, unsigned num, ESetType t)
{
	unsigned grp = get_grp(row, col);

	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);

	DEBUG_MSG("Set (%d, %d) to %d (%d)\n", row, col, num, t);

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

