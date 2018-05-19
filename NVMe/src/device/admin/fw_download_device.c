#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_FwDownload(NVME_QID sqid, NVME_QID cqid)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	if (NVME_NSID_NONE != sqe->NSID) {
		Device_SetNvmeStatus(cq, NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT);
        	Device_ChangeState(eDeviceState_ReturnStatus);
	        return TRUE;
	}

	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

