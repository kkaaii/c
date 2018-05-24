#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_Identify(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	do {
		if (0 != sqe->CDW10.identify.CNTID) {
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			break;
		}

		switch (sqe->CDW10.identify.CNS) {
		case eCNS_NamespaceData:
			if (NVME_NSID_INVALID <= sqe->NSID) {
				status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			}
			break;

		case eCNS_ControllerData:
			if (NVME_NSID_NONE != sqe->NSID) {
				status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			}
			break;

		case eCNS_ActiveNsidList:
			if (NVME_NSID_GLOBAL == sqe->NSID || ~1u == sqe->NSID) {
				status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			}
			break;

		default:
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			break;
		}
	} while (FALSE);

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

