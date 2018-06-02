#ifndef	_NVME_DEBUG_H
#define	_NVME_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#define	DBG_LVL_ALWAYS	0
#define	DBG_LVL_FATAL	1
#define	DBG_LVL_ERROR	2
#define	DBG_LVL_DEBUG	3

#define	DBG_LEVEL	DBG_LVL_ERROR
//#define	DBG_LEVEL	DBG_LVL_DEBUG

#define	MSG(level, ...)	do {if ((level) <= DBG_LEVEL) printf(__VA_ARGS__);} while(0)

#define	ALL_MSG(...)	MSG(DBG_LVL_ALWAYS, __VA_ARGS__)
#define	DBG_MSG(...)	MSG(DBG_LVL_DEBUG, __VA_ARGS__)
#define	ERR_MSG(...)	MSG(DBG_LVL_ERROR, __VA_ARGS__)

#define	ENTER()		DBG_MSG("+++ %s\n", __FUNCTION__)
#define	LEAVE()		DBG_MSG("--- %s:%d\n", __FUNCTION__, __LINE__)
#define	TRACE()		DBG_MSG("*** %s:%d\n", __FUNCTION__, __LINE__)

#define	WATCH(x)	DBG_MSG("    %s = 0x%lx (%lu)\n", #x, (UINT64)(x), (UINT64)(x))

#define	ASSERT(x)	do {					\
	if (!(x)) {						\
		ERR_MSG("!!! assert failure: %s\n", #x);	\
		ERR_MSG("%s:%d\n", __FILE__, __LINE__);		\
		exit(1);					\
	}							\
} while (FALSE)

#endif	/* _NVME_DEBUG_H */

