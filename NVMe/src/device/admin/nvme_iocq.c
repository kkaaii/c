/******************************************************************************
** File Name: nvme_iocq.c
** Author:
** Creation Time: Fri Jan 12 00:53:56 2018
*/
#include <stdio.h>
#include <string.h>
#include "nvme.h"
#include "nvme_iocq.h"

static
NVME_CQ	cqs[NVME_MAX_QUEUES];

CC_STATIC_ALWAYS_INLINE
NVME_CQ	*NvmeCq_Get(UINT16 cqid)
{
	if (cqid < DIM(cqs))
		return &cqs[cqid];

	return NULL;
}

void	NvmeCq_Init(void)
{
	memset(cqs, 0, sizeof cqs);
}

BOOL	NvmeCq_IsValid(UINT16 cqid)
{
	NVME_CQ	*cq = NvmeCq_Get(cqid);
	return (NULL != cq && cq->valid) ? TRUE : FALSE;
}

NVME_STATUS
NvmeCq_ValidateCreation(NVME_SQ_ENTRY *entry)
{
	UINT16	cqid;
	NVME_CQ	*cq;
	UINT16	qsize;
	UINT16	iv;

	cqid = entry->cdw10.createq.qid;
	cq = NvmeCq_Get(cqid);
	if (NVME_CQID_ADMIN == cqid || NULL == cq || cq->valid) {
		return NVME_STATUS_INVALID_QID;
	}

	qsize = entry->cdw10.createq.qsize;
	if (!NVME_QSIZE_IS_VALID(qsize)) {
		return NVME_STATUS_INVALID_QSIZE;
	}

	iv = entry->cdw11.createIoCq.iv;
	if (0 && iv) {		/* TODO: validate iv */
		return NVME_STATUS_INVALID_INT_VECTOR;
	}

	return NVME_STATUS_SUCCESSFUL_COMPLETION;
}

void
NvmeCq_Create(NVME_SQ_ENTRY *entry)
{
	UINT16	qsize = entry->cdw10.createq.qsize;
	UINT16	cqid = entry->cdw10.createq.qid;
	NVME_CQ	*cq = NvmeCq_Get(cqid);


	NVME_QUEUE_INIT(&cq->q, qsize);
	cq->iv = entry->cdw11.createIoCq.iv;
	cq->ien = entry->cdw11.createIoCq.ien;
	cq->valid = 1;
	cq->ref	= 0;
}

NVME_STATUS
NvmeCq_ValidateDeletion(NVME_SQ_ENTRY *entry)
{
	UINT16	cqid = entry->cdw10.deleteq.qid;
	NVME_CQ	*cq = NvmeCq_Get(cqid);

	if (NVME_CQID_ADMIN == cqid || NULL == cq || !cq->valid) {
		return NVME_STATUS_INVALID_QID;
	}

	if (0 != cq->ref) {
		return NVME_STATUS_INVALID_QUEUE_DELETION;
	}

	return NVME_STATUS_SUCCESSFUL_COMPLETION;
}

void
NvmeCq_Delete(NVME_SQ_ENTRY *entry)
{
	UINT16	cqid = entry->cdw10.deleteq.qid;
	NVME_CQ	*cq = NvmeCq_Get(cqid);

	cq->valid = 0;
}

void	NvmeCq_Bind(UINT16 cqid)
{
	NVME_CQ	*cq = NvmeCq_Get(cqid);

	++cq->ref;
}

void	NvmeCq_Unbind(UINT16 cqid)
{
	NVME_CQ	*cq = NvmeCq_Get(cqid);

	--cq->ref;
}

