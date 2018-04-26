#ifndef	_NVME_DEBUG_H
#define	_NVME_DEBUG_H

#include <stdio.h>

#define	ENTER()		printf("+++ %s\n", __FUNCTION__)
#define	LEAVE()		printf("--- %s\n", __FUNCTION__)

#define	WATCH(x)	printf("    %s = 0x%lx (%lu)\n", #x, (UINT64)(x), (UINT64)(x))

#endif	/* _NVME_DEBUG_H */

