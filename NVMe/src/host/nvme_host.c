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

CC_STATIC
BOOL Host_BuildPrpList(UINT64 *prp, UINT64 addr, UINT32 entries)
{
	UINT32	n = HOST_PAGE_SIZE / sizeof (UINT64) - 1;
	UINT32	pages = (entries + n - 1) / n;
	UINT64	*entry;
	UINT32	i;

	ASSERT(entries >= 2);

	entry = (UINT64 *)Platform_MemAlign(HOST_PAGE_SIZE, HOST_PAGE_SIZE * pages);
	if (NULL == entry) {
		return FALSE;
	}

	for (; entries > 0; entries -= n) {
		*prp = CAST_PTR(UINT64)(entry);

		if (n > entries)
			n = entries;

		for (i = 0; i < n; ++i) {
			*entry++ = addr;
			addr += HOST_PAGE_SIZE;
		}
		prp = entry++;
	}

	return TRUE;
}

BOOL Host_BuildPRP1(NVME_SQE *sqe, void *buf, UINT32 bytes)
{
	UINT64	s = CAST_PTR(UINT64)(buf);
	UINT64	e = s + bytes - 1;
	UINT32	n = (UINT32)((e / HOST_PAGE_SIZE) - (s / HOST_PAGE_SIZE) + 1);

	ASSERT(0 == (s % HOST_PAGE_SIZE));

	switch (n) {
	case 0:
		break;

	case 1:	/* PRP1 entry */
		sqe->DPTR.PRP1 = s;
		break;

	default:
		Host_BuildPrpList(&sqe->DPTR.PRP1, s, n);
		return FALSE;
	}

	return TRUE;
}

BOOL Host_BuildPRPs(NVME_SQE *sqe, void *buf, UINT64 bytes)
{
	UINT64	s = CAST_PTR(UINT64)(buf);
	UINT64	e = s + bytes - 1;
	UINT32	n = (UINT32)((e / HOST_PAGE_SIZE) - (s / HOST_PAGE_SIZE) + 1);

	sqe->DPTR.PRP1 = s;
	s = (s + HOST_PAGE_SIZE) & ~(HOST_PAGE_SIZE - 1);

	switch (n) {
	case 0:
		break;

	case 1:	/* PRP1 entry only */
		sqe->DPTR.PRP2 = 0;
		break;

	case 2:	/* PRP1 entry & PRP2 entry */
		sqe->DPTR.PRP2 = s;
		return FALSE;

	default:/* PRP1 entry & PRP2 list */
		Host_BuildPrpList(&sqe->DPTR.PRP2, s, n - 1);
		return FALSE;
	}

	return TRUE;
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
