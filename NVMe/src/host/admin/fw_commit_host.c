#include "nvme/nvme.h"
#include "nvme_host.h"

NVME_CID
Host_FwCommit(
	NVME_NSID	nsid,
        UINT8           ca,
        UINT8           fs)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_FW_COMMIT;
	sqe->NSID = nsid;
	sqe->CDW10.firmwareCommit.CA = ca;
	sqe->CDW10.firmwareCommit.FS = fs;

	return Host_IssueCommand(asq);
}

