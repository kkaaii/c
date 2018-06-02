#include "nvme/nvme.h"
#include "nvme_host.h"

NVME_CID
Host_CreateIoCompletionQueue(
	NVME_NSID	nsid,
	NVME_QID	cqid,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_CREATE_IOCQ;
	sqe->NSID = nsid;

	sqe->CDW10.createIoCompletionQueue.QID = cqid;
	sqe->CDW10.createIoCompletionQueue.QSIZE = ZERO_BASED(bytes / sizeof (NVME_CQE));

#if 1	/* TODO */
	sqe->CDW11.createIoCompletionQueue.IEN = 0;
	sqe->CDW11.createIoCompletionQueue.IV = 0;
#endif

	sqe->CDW11.createIoCompletionQueue.PC = Host_BuildPRP1(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

