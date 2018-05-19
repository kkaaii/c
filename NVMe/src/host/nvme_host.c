#include "nvme/nvme.h"
#include "nvme_host.h"
#include "platform.h"

CC_STATIC	NVME_CID	cid = 0;

CC_STATIC_ALWAYS_INLINE
NVME_CID Host_AllocCommandId(void)
{
	return cid++;
}

CC_STATIC_ALWAYS_INLINE
NVME_CID _IssueCommand(NVME_QUEUE *sq)
{
	ASSERT(NULL != sq);
	ASSERT(!NVME_QUEUE_IS_FULL(sq));

	NVME_SQE	*sqe = (NVME_SQE *)(sq->base) + sq->tail;
	NVME_CID	cid = Host_AllocCommandId();
	sqe->CDW0.CID = cid;
	NVME_QUEUE_INC_TAIL(sq);

	HOST_DBG_MSG("(%02d|%02d)(--|--) %04xh\n", sq->head, sq->tail, cid);
	return cid;
}

NVME_CID Host_IssueCommand(NVME_QUEUE *sq)
{
	return _IssueCommand(sq);
}

BOOL Host_BuildPRP1(NVME_SQE *sqe, void *buf, UINT32 bytes)
{
	UINT64	s = CAST_PTR(UINT64)(buf);
	UINT64	e = s + bytes - 1;
	UINT32	n = (UINT32)((e >> HOST_PAGE_SIZE_BITS) - (s >> HOST_PAGE_SIZE_BITS) + 1);

	ASSERT(n < HOST_PAGE_SIZE / sizeof (UINT64));

	if (1 == n) {
		sqe->DPTR.PRP1 = s;
		return TRUE;
	}

	UINT64	*p = (UINT64 *)malloc_align(HOST_PAGE_SIZE, HOST_PAGE_SIZE);
	while (n--) {
		*p++ = s;
		s = (s + HOST_PAGE_SIZE) & ~(UINT64)HOST_PAGE_SIZE_MASK;
	}

	sqe->DPTR.PRP1 = CAST_PTR(UINT64)(p);
	return FALSE;
}

NVME_CQE *Host_WaitForCompletion(NVME_QID cqid, NVME_CID cid)
{
	NVME_QUEUE	*cq = Host_GetCompletionQueue(cqid);
	NVME_CQE	*cqe;

	ASSERT(NULL != cq);
	do {
		cqe = Host_CheckResponse(cq);
	} while (NULL == cqe || cid != cqe->dw3.CID);

	return cqe;
}

NVME_CID Host_CreateIoCq(NVME_QUEUE *asq, NVME_QID cqid, void *buf, UINT32 bytes)
{
	ASSERT(NULL != asq);
	ASSERT(!NVME_QUEUE_IS_FULL(asq));

	NVME_SQE	*sqe = (NVME_SQE *)asq->base + asq->tail;

	sqe->CDW0.OPC = NVME_OPC_ADMIN_CREATE_IOCQ;

	sqe->CDW10.createq.QID = cqid;
	sqe->CDW10.createq.QSIZE = ZERO_BASED(bytes / sizeof (NVME_CQE));

#if 1	/* TODO */
	sqe->CDW11.iocq.IEN = 0;
	sqe->CDW11.iocq.IV = 0;
#endif

	sqe->CDW11.iocq.PC = Host_BuildPRP1(sqe, buf, bytes);

	return _IssueCommand(asq);
}

NVME_CID Host_CreateIoSq(NVME_QUEUE *asq, NVME_QID sqid, void *buf, UINT32 bytes)
{
	ASSERT(NULL != asq);
	ASSERT(!NVME_QUEUE_IS_FULL(asq));

	NVME_SQE	*sqe = (NVME_SQE *)asq->base + asq->tail;

	sqe->CDW0.OPC = NVME_OPC_ADMIN_CREATE_IOSQ;

	sqe->CDW10.createq.QID = sqid;
	sqe->CDW10.createq.QSIZE = ZERO_BASED(bytes / sizeof (NVME_SQE));

#if 1	/* TODO */
	sqe->CDW11.iosq.QPRIO = 0;
	sqe->CDW11.iosq.CQID = 0;
#endif

	sqe->CDW11.iocq.PC = Host_BuildPRP1(sqe, buf, bytes);

	return _IssueCommand(asq);
}
