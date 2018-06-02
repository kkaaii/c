#include "nvme/nvme.h"
#include "nvme_host.h"

NVME_CID
Host_DeleteIoSubmissionQueue(
	NVME_NSID	nsid,
	NVME_QID	qid)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);
	ASSERT(NULL != sqe);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_DELETE_IOSQ;
	sqe->NSID = nsid;

	sqe->CDW10.deleteIoSubmissionQueue.QID = qid;

	return Host_IssueCommand(asq);
}

