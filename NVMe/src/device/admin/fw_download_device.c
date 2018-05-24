#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_FwDownload(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	if (NVME_NSID_NONE != sqe->NSID) {
		status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
	}

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

