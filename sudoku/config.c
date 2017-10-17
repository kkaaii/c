/******************************************************************************
** File Name: config.c
** Author:
** Creation Time: Mon Oct 16 07:10:12 2017
*/
#include <stdio.h>
#include "config.h"
#include "core.h"
#include "colors.h"

static const char *bg_colors[] = {
	BG_IMAGENTA,
	BG_IGREEN,
	BG_IBLUE
};

static unsigned c[1 + GRPS];

const char *get_color(unsigned grp)
{
	return bg_colors[c[grp]];
}

int load_config(const char *config)
{
	FILE *fp = fopen(config, "r");
	unsigned row, col;
	unsigned grp;
	unsigned num;

	if (NULL == fp)
		return 0;

	DEBUG_MSG("loading colors...\n");
	for (grp = 1; grp <= GRPS; ++grp) {
		c[grp] = fgetc(fp) - '0';
	}
	fgetc(fp);

	DEBUG_MSG("loading grps...\n");
	fgetc(fp);
	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			grp = fgetc(fp) - '0';
			set_grp(row, col, grp);
		}
		fgetc(fp);
	}

	DEBUG_MSG("loading data...\n");
	fgetc(fp);
	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			num = fgetc(fp);
			if ('-' != num) {
				set_num(row, col, num - '0', eInit);
			}
		}
		fgetc(fp);
	}

	fclose(fp);
	return 1;
}

