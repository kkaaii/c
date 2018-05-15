#ifndef	_NVME_HOST_H
#define	_NVME_HOST_H

#define	MODULE_NAME		"[ host ]"

#define	HOST_PAGE_SIZE_BITS	12
#define	HOST_PAGE_SIZE		(1u << HOST_PAGE_SIZE_BITS)
#define	HOST_PAGE_SIZE_MASK	(HOST_PAGE_SIZE - 1)

#define	HOST_DBG_MSG(...)	DBG_MSG(MODULE_NAME __VA_ARGS__)
#define	HOST_PRN_MSG(...)	DBG_MSG(MODULE_NAME __VA_ARGS__)

NVME_QUEUE *Host_GetCompletionQueue(NVME_QID cqid);
NVME_QUEUE *Host_GetSubmissionQueue(NVME_QID sqid);

void Host_RingBoorbell_CQH(NVME_QID cqid);
void Host_RingBoorbell_SQT(NVME_QID sqid);

BOOL Host_BuildPRP1(NVME_SQE *sqe, void *buf, UINT32 bytes);
NVME_CID Host_IssueCommand(NVME_QUEUE *sq);

CC_STATIC_ALWAYS_INLINE
NVME_SQE *Host_GetSubmissionQueueEntry(NVME_QUEUE *sq)
{
	ASSERT(NULL != sq);
	ASSERT(!NVME_QUEUE_IS_FULL(sq));

	return (NVME_SQE *)(sq->base) + sq->tail;
}

NVME_CQE *Host_CheckResponse(NVME_QUEUE *cq);

#endif	/* _NVME_HOST_H */

