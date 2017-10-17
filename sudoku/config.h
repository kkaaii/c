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

#define	BITS	4

#define	ROWS	N
#define	COLS	N
#define	GRPS	N

#define	ALL		MASK(1, N)

#ifdef DEBUG
	#include <stdio.h>
	#include <stdlib.h>

	#define	DEBUG_MSG	printf

	#define	DEBUG_ASSERT(x)	do {											\
		if (!(x)) {															\
			DEBUG_MSG("Assertion: \"%s\", %s:%d\n", #x, __FILE__, __LINE__);\
			exit(-1);														\
		}																	\
	} while (0)
#else
	#define	DEBUG_MSG(...)
	#define	DEBUG_ASSERT(...)
#endif

const char *get_color(unsigned grp);
int load_config(const char *config);

#endif	/* _CONFIG_H */

