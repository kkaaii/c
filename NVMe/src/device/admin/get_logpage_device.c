#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_GetLogPage(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	switch (sqe->CDW10.getLogPage.LID) {
	case eLID_CommandEffectsLog:
		if (0 == LPA_SUPPORT_COMMAND_EFFECTS_LOG) {
			status = eSF_DoNotRetry | eSF_InvalidLogPage;
			break;
		}
		/* fall through */

	case eLID_ErrorInformation:
	case eLID_FirmwareSlotInformation:
		if (NVME_NSID_GLOBAL != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
		}
		break;

	case eLID_SmartHealthInformation:
		if (NVME_NSID_GLOBAL != sqe->NSID) {
			if (0 == LPA_SUPPORT_NS_BASED_SMART_LOG ||
				NVME_NSID_NONE == sqe->NSID ||
				NVME_NSID_INVALID <= sqe->NSID) {
				status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			}
		}
		break;

	default:
		status = eSF_DoNotRetry | eSF_InvalidLogPage;
		break;
	}

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

