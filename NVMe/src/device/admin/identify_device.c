#include "nvme/nvme.h"
#include "nvme_device.h"

BOOL Device_Identify(NVME_QID sqid, NVME_QID cqid)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	if (0 != sqe->CDW10.identify.CNTID) {
		Device_SetNvmeStatus(cq, eSF_InvalidFieldInCommand);
		Device_ChangeState(eDeviceState_ReturnStatus);
		return TRUE;
	}

	switch (sqe->CDW10.identify.CNS) {
	case eCNS_NamespaceData:
		if (NVME_NSID_INVALID <= sqe->NSID) {
			Device_SetNvmeStatus(cq, eSF_InvalidNamespaceOrFormat);
		}
		Device_ChangeState(eDeviceState_ReturnStatus);
		break;

	case eCNS_ControllerData:
		if (NVME_NSID_NONE != sqe->NSID) {
			Device_SetNvmeStatus(cq, eSF_InvalidNamespaceOrFormat);
		}
		Device_ChangeState(eDeviceState_ReturnStatus);
		break;

	case eCNS_ActiveNsidList:
		if (NVME_NSID_GLOBAL == sqe->NSID || ~1u == sqe->NSID) {
			Device_SetNvmeStatus(cq, eSF_InvalidNamespaceOrFormat);
		}
		Device_ChangeState(eDeviceState_ReturnStatus);
		break;

	default:
		Device_SetNvmeStatus(cq, eSF_InvalidFieldInCommand);
		Device_ChangeState(eDeviceState_ReturnStatus);
		break;
	}

	return TRUE;
}

