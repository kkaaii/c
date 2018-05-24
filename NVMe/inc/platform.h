#ifndef	_PLATFORM_H
#define	_PLATFORM_H

void *malloc_align(UINT32 alignment, UINT32 size);

BOOL HostTest_CheckStatus(NVME_STATUS expected, NVME_STATUS actual);

#endif	/* _PLATFORM_H */

