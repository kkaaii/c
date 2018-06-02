#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_FwCommit(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	switch (sqe->CDW10.firmwareCommit.CA) {
	case eCA_ACTIVATE_IMMEDIATELY:
		if (0 == FRMW_SUPPORT_IMMEDIATE_FW_ACTIVATION) {
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			Device_SetNvmeStatus(cq, status);
        		Device_ChangeState(eDeviceState_ReturnStatus);
			return TRUE;
		}
	case eCA_ACTIVATE_WITH_RESET:
	case eCA_COMMIT_AND_ACTIVATE_WITH_RESET:
	case eCA_COMMIT_NO_ACTIVATION:
		break;

	default:
		status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
		Device_SetNvmeStatus(cq, status);
        	Device_ChangeState(eDeviceState_ReturnStatus);
		return TRUE;
	}

	do {
		if (NVME_NSID_NONE != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			break;
		}

		if (FRMW_NUMBER_OF_FW_SLOTS < sqe->CDW10.firmwareCommit.FS) {
			status = eSF_DoNotRetry | eSF_InvalidFirmwareSlot;
			break;
		}

		status = eSF_InvalidFirmwareImage;
	} while (FALSE);

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

