#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_FwCommit(NVME_QID sqid, NVME_QID cqid)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	switch (sqe->CDW10.fwCommit.CA) {
	case eCA_ACTIVATE_IMMEDIATELY:
		if (0 == (FRMW & BIT(4))) {
			Device_SetNvmeStatus(cq, eSF_InvalidFieldInCommand);
        		Device_ChangeState(eDeviceState_ReturnStatus);
			return TRUE;
		}
	case eCA_ACTIVATE_WITH_RESET:
	case eCA_COMMIT_AND_ACTIVATE_WITH_RESET:
	case eCA_COMMIT_NO_ACTIVATION:
		break;

	default:
		Device_SetNvmeStatus(cq, eSF_InvalidFieldInCommand);
       		Device_ChangeState(eDeviceState_ReturnStatus);
		return TRUE;
	}

	if (NVME_NSID_NONE != sqe->NSID) {
		Device_SetNvmeStatus(cq, eSF_InvalidNamespaceOrFormat);
        	Device_ChangeState(eDeviceState_ReturnStatus);
	        return TRUE;
	}

	if (1 < sqe->CDW10.fwCommit.FS) {
		Device_SetNvmeStatus(cq, eSF_InvalidFirmwareSlot);
		Device_ChangeState(eDeviceState_ReturnStatus);
		return TRUE;
	}

	Device_SetNvmeStatus(cq, eSF_InvalidFirmwareImage);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

