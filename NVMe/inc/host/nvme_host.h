#ifndef	_NVME_HOST_H
#define	_NVME_HOST_H

#define	HOST_PAGE_SIZE_BITS	12
#define	HOST_PAGE_SIZE		(1u << HOST_PAGE_SIZE_BITS)
#define	HOST_PAGE_SIZE_MASK	(HOST_PAGE_SIZE - 1)

#define	HOST			"[ host ]"
#define	HOST_DBG_MSG(...)	DBG_MSG(HOST __VA_ARGS__)
#define	HOST_ERR_MSG(...)	ERR_MSG(HOST __VA_ARGS__)
#define	HOST_MSG(...)		ALL_MSG(HOST __VA_ARGS__)

NVME_QUEUE *Host_GetCompletionQueue(NVME_QID cqid);
NVME_QUEUE *Host_GetSubmissionQueue(NVME_QID sqid);

NVME_QUEUE *Host_InitCompletionQueue(NVME_QID cqid, void *buf, UINT16 qsize);
NVME_QUEUE *Host_InitSubmissionQueue(NVME_QID sqid, void *buf, UINT16 qsize);

void Host_RingBoorbell_CQH(NVME_QID cqid);
void Host_RingBoorbell_SQT(NVME_QID sqid);

BOOL Host_BuildPRP1(NVME_SQE *sqe, void *buf, UINT32 bytes);
BOOL Host_BuildPRPs(NVME_SQE *sqe, void *buf, UINT64 bytes);

NVME_CID Host_IssueCommand(NVME_QUEUE *sq);

CC_STATIC_ALWAYS_INLINE
NVME_SQE *Host_GetSubmissionQueueEntry(NVME_QUEUE *sq)
{
	ASSERT(NULL != sq);
	ASSERT(!NVME_QUEUE_IS_FULL(sq));

	return (NVME_SQE *)(sq->base) + sq->tail;
}

NVME_CQE *Host_WaitForCompletion(NVME_QID cqid, NVME_CID cid);
NVME_CQE *Host_CheckResponse(NVME_QUEUE *cq);

#include "platform.h"
#include "admin/identify_host.h"
#include "admin/get_logpage_host.h"
#include "admin/fw_download_host.h"
#include "admin/fw_commit_host.h"
#include "admin/features_host.h"
#include "admin/create_iocq_host.h"
#include "admin/create_iosq_host.h"
#include "admin/delete_iosq_host.h"
#include "admin/delete_iocq_host.h"

#endif	/* _NVME_HOST_H */

