#include "nvme/nvme.h"
#include "nvme_device.h"

CC_STATIC	UINT32	fidEnabled[] = {
	0x00011FF6,	/* 00h - 1Fh */
	0x00000000,	/* 20h - 3Fh */
	0x00000000,	/* 40h - 5Fh */
	0x00000000,	/* 60h - 7Fh */
	0x00000000,	/* 80h - 9Fh */
	0x00000000,	/* A0h - BFh */
	0x00000000,	/* C0h - DFh */
	0x00000000,	/* E0h - FFh */
};

CC_STATIC	struct {
	UINT32	saveable	: 1;
	UINT32	ns_specific	: 1;
	UINT32	changeable	: 1;
	UINT32	reserved	: 29;
} fidConfig[] = {
	{.saveable = 0, .ns_specific = 0, .changeable = 0, .reserved = 0},	// 00h
	{.saveable = 0, .ns_specific = 0, .changeable = 0, .reserved = 0},	// 01h
	{.saveable = 1, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 02h
	{.saveable = 0, .ns_specific = 0, .changeable = 0, .reserved = 0},	// 03h
	{.saveable = 0, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 04h
	{.saveable = 0, .ns_specific = 1, .changeable = 1, .reserved = 0},	// 05h
	{.saveable = 0, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 06h
	{.saveable = 0, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 07h
	{.saveable = 0, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 08h
	{.saveable = 0, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 09h
	{.saveable = 0, .ns_specific = 0, .changeable = 0, .reserved = 0},	// 0Ah
	{.saveable = 1, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 0Bh
	{.saveable = 1, .ns_specific = 0, .changeable = 1, .reserved = 0},	// 0Ch
};

BOOL Device_GetFeatures(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);

	if (NVME_NSID_INVALID <= sqe->NSID && NVME_NSID_GLOBAL != sqe->NSID) {
		status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
	}

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

BOOL Device_SetFeatures(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);
	NVME_FID	fid = sqe->CDW10.setFeatures.FID;

	do {
		if (sqe->CDW10.setFeatures.SV && !fidConfig[fid].saveable) {
			status = eSF_DoNotRetry | eSF_FeatureIdentifierNotSaveable;
			break;
		}

		if (NVME_NSID_NONE != sqe->NSID && NVME_NSID_GLOBAL != sqe->NSID) {
			if (NVME_NSID_INVALID <= sqe->NSID) {
				status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
				break;
			}

			if (!fidConfig[fid].ns_specific) {
				status = eSF_DoNotRetry | eSF_FeatureNotNamespaceSpecific;
				break;
			}
		}
	} while (FALSE);

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

