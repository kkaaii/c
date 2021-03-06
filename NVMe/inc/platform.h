#ifndef	_PLATFORM_H
#define	_PLATFORM_H

void *Platform_MemAlign(UINT32 alignment, UINT32 size);

BOOL HostTest_CheckStatus(NVME_STATUS expected, NVME_STATUS actual);
NVME_STATUS HostTest_SyncExecuteAdmin(NVME_CID cid);

#endif	/* _PLATFORM_H */

