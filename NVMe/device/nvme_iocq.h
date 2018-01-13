/******************************************************************************
** File Name: nvme_iocq.h
** Author:
** Creation Time: Fri Jan 12 00:53:56 2018
*/
#ifndef	_NVME_IOCQ_H
#define	_NVME_IOCQ_H

#include "nvme_queue.h"

#define	NVME_CQ_IS_EMPTY(cq)	NVME_QUEUE_IS_EMPTY(&(cq)->q)
#define	NVME_CQ_IS_FULL(cq)	NVME_QUEUE_IS_FULL(&(cq)->q)

typedef	struct {
	NVME_QUEUE	q;
	UINT16		iv;

	UINT16		ref	: 14;
	UINT16		valid	: 1;
	UINT16		ien	: 1;
} NVME_CQ;

NVME_STATUS	NvmeCq_Create(NVME_SQ_ENTRY *entry);
NVME_STATUS	NvmeCq_Delete(NVME_SQ_ENTRY *entry);

BOOL		NvmeCq_IsValid(UINT16 cqid);
void		NvmeCq_Bind(UINT16 cqid);
void		NvmeCq_Unbind(UINT16 cqid);

#endif	/* _NVME_IOCQ_H */

