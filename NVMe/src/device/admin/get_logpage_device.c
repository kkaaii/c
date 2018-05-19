#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_GetLogPage(NVME_QID sqid, NVME_QID cqid)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	if (NVME_NSID_GLOBAL != sqe->NSID) {
		if (eLID_SMART_HEALTH_INFO != sqe->CDW10.getLogPage.LID ||
			NVME_NSID_NONE == sqe->NSID ||
			NVME_NSID_INVALID <= sqe->NSID) {
			Device_SetNvmeStatus(cq, eSF_InvalidNamespaceOrFormat);
		}
	}

	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

