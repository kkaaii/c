/******************************************************************************
** File Name: config.h
** Author:
** Creation Time: Mon Oct 16 07:07:44 2017
*/
#ifndef _CONFIG_H
#define	_CONFIG_H

#define	BIT(x)	(1u << (x))
#define	MASK(off,bits)	((BIT(bits) - 1) << (off))

#define	N		9

#define	ROWS	N
#define	COLS	N
#define	GRPS	N

#define	ALL		MASK(1, N)

const char *get_color(unsigned grp);
int load_config(const char *config);

#endif	/* _CONFIG_H */

