/******************************************************************************
** File Name: core.c
** Author:
** Creation Time: Mon Oct 16 06:54:52 2017
*/
#include "config.h"
#include "core.h"

typedef union cell_t {
	unsigned all;
	struct {
		unsigned	num : BITS;
		unsigned	grp : BITS;
		unsigned	bmp : N + 1;
	};
} cell_t;

static cell_t cells[ROWS][COLS];

static unsigned _ctz(unsigned x)
{
	return __builtin_ctz(x);
}

static int is_set(unsigned row, unsigned col)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);

	return 0 != cells[row][col].num;
}

static void _clr(unsigned row, unsigned col, unsigned num, ESetType t)
{
	unsigned cell;

	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);
	DEBUG_ASSERT(0 < num && num <= N);

	if (!is_set(row, col)) {
		cell = cells[row][col].bmp &= ~BIT(num);
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

	cells[row][col].bmp = BIT(num);
	cells[row][col].num = num;
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

	return cells[row][col].grp;
}

unsigned get_num(unsigned row, unsigned col)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);

	return cells[row][col].num;
}

void set_grp(unsigned row, unsigned col, unsigned grp)
{
	DEBUG_ASSERT(row < ROWS);
	DEBUG_ASSERT(col < COLS);
	DEBUG_ASSERT(grp - 1 < GRPS);

	cells[row][col].grp = grp;
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
			cells[row][col].all = 0;
			cells[row][col].bmp = ALL;
		}
	}
}

