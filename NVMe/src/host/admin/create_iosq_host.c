#include "nvme/nvme.h"
#include "nvme_host.h"

NVME_CID
Host_CreateIoSubmissionQueue(
	NVME_NSID	nsid,
	NVME_QID	sqid,
	NVME_QID	cqid,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);
	ASSERT(NULL != sqe);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_CREATE_IOSQ;
	sqe->NSID = nsid;

	sqe->CDW10.createIoSubmissionQueue.QID = sqid;
	sqe->CDW10.createIoSubmissionQueue.QSIZE = ZERO_BASED(bytes / sizeof (NVME_SQE));

#if 1	/* TODO */
	sqe->CDW11.createIoSubmissionQueue.QPRIO = 0;
#endif

	sqe->CDW11.createIoSubmissionQueue.CQID = cqid;
	sqe->CDW11.createIoSubmissionQueue.PC = Host_BuildPRP1(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

