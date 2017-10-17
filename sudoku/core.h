/******************************************************************************
** File Name: core.h
** Author:
** Creation Time: Mon Oct 16 06:52:42 2017
*/
#ifndef _CORE_H
#define	_CORE_H

typedef enum {
	eInit,
	eUniqueInRow,
	eUniqueInCol,
	eUniqueInGrp,
} ESetType;

void init(void);

unsigned get_grp(unsigned row, unsigned col);
unsigned get_num(unsigned row, unsigned col);

void set_grp(unsigned row, unsigned col, unsigned grp);
void set_num(unsigned row, unsigned col, unsigned num, ESetType t);

#endif	/* _CORE_H */

