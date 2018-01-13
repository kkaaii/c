/******************************************************************************
** File Name: nvme_iosq.c
** Author:
** Creation Time: Thu Jan 11 06:29:29 2018
*/
#include <stdio.h>
#include <string.h>
#include "nvme.h"
#include "nvme_iocq.h"
#include "nvme_iosq.h"

static
NVME_SQ	sqs[NVME_MAX_QUEUES];

CC_STATIC_ALWAYS_INLINE
NVME_SQ	*NvmeSq_Get(UINT16 sqid)
{
	if (sqid < DIM(sqs))
		return &sqs[sqid];

	return NULL;
}

void	NvmeSq_Init(void)
{
	memset(sqs, 0, sizeof sqs);
}

BOOL	NvmeSq_IsValid(UINT16 sqid)
{
	NVME_SQ	*sq = NvmeSq_Get(sqid);
	return (NULL != sq && sq->valid) ? TRUE : FALSE;
}

NVME_STATUS	NvmeSq_Create(NVME_SQ_ENTRY *entry)
{
	UINT16	cqid;
	UINT16	sqid;
	UINT16	qsize;
	NVME_SQ	*sq;

	cqid = entry->cdw11.createIoSq.cqid;
	if (!NvmeCq_IsValid(cqid)) {
		return NVME_STATUS_CQ_INVALID;
	}

	sqid = entry->cdw10.createq.qid;
	sq = NvmeSq_Get(sqid);
	if (0 == sqid || NULL == sq || sq->valid) {
		return NVME_STATUS_INVALID_QID;
	}

	qsize = entry->cdw10.createq.qsize;
	if (!NVME_QSIZE_IS_VALID(qsize)) {
		return NVME_STATUS_INVALID_QSIZE;
	}

	NVME_QUEUE_INIT(&sq->q, qsize);
	sq->cqid	= cqid;
	sq->valid	= 1;
	sq->prio	= entry->cdw11.createIoSq.qprio;

	NvmeCq_Bind(cqid);
	return NVME_STATUS_SUCCESSFUL_COMPLETION;
}

NVME_STATUS	NvmeSq_Delete(NVME_SQ_ENTRY *cmd)
{
	NVME_SQ	*sq = NvmeSq_Get(cmd->cdw10.deleteq.qid);

	if (NULL == sq || !sq->valid) {
		return NVME_STATUS_INVALID_QID;
	}

	sq->valid	= 0;

	NvmeCq_Unbind(sq->cqid);
	return NVME_STATUS_SUCCESSFUL_COMPLETION;
}

