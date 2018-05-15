#ifndef	_NVME_DEBUG_H
#define	_NVME_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#define	DBG_MSG		printf

#define	ENTER()		DBG_MSG(MODULE_NAME "+++ %s\n", __FUNCTION__)
#define	LEAVE()		DBG_MSG(MODULE_NAME "--- %s:%d\n", __FUNCTION__, __LINE__)
#define	TRACE()		DBG_MSG(MODULE_NAME "*** %s:%d\n", __FUNCTION__, __LINE__)

#define	WATCH(x)	DBG_MSG("    %s = 0x%lx (%lu)\n", #x, (UINT64)(x), (UINT64)(x))

#define	ASSERT(x)	do {					\
	if (!(x)) {						\
		DBG_MSG("!!! assert failure: %s\n", #x);	\
		exit(1);					\
	}							\
} while (FALSE)

#endif	/* _NVME_DEBUG_H */

