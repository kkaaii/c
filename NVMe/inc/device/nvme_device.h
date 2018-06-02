#ifndef	_NVME_DEVICE_H
#define	_NVME_DEVICE_H

#define	DEV			"\t\t\t\t\t[device]"

#define	DEV_DBG_MSG(...)	DBG_MSG(DEV __VA_ARGS__)
#define	DEV_ERR_MSG(...)	ERR_MSG(DEV __VA_ARGS__)
#define	DEV_MSG(...)		ALL_MSG(DEV __VA_ARGS__)

#define	MQES			64
#define	MAX_QUEUES		8

enum {
	NVME_NSID_1		= 1,
	NVME_NSID_INVALID,
	NVME_NSID_ACTIVE	= NVME_NSID_1,
};

typedef enum {
	eDeviceState_FetchCommand,
	eDeviceState_DeleteIoSq,
	eDeviceState_CreateIoSq,
	eDeviceState_DeleteIoCq,
	eDeviceState_CreateIoCq,
	eDeviceState_Identify,
	eDeviceState_GetLogPage,
	eDeviceState_FwDownload,
	eDeviceState_FwCommit,
	eDeviceState_GetFeatures,
	eDeviceState_SetFeatures,
	eDeviceState_ReturnStatus
} EDeviceState;

NVME_QUEUE *Device_GetCompletionQueue(NVME_QID cqid);
NVME_QUEUE *Device_GetSubmissionQueue(NVME_QID sqid);

void Device_InitCompletionQueue(NVME_QID cqid, void *buf, UINT16 qsize);
void Device_InitSubmissionQueue(NVME_QID sqid, void *buf, UINT16 qsize);

CC_STATIC_ALWAYS_INLINE
NVME_CQE *Device_GetCompletionQueueEntry(NVME_QUEUE *cq)
{
	ASSERT(NULL != cq);
	ASSERT(!NVME_QUEUE_IS_FULL(cq));

	return (NVME_CQE *)(cq->base) + cq->tail;
}

CC_STATIC_ALWAYS_INLINE
NVME_SQE *Device_GetSubmissionQueueEntry(NVME_QUEUE *sq)
{
	ASSERT(NULL != sq);
	ASSERT(!NVME_QUEUE_IS_EMPTY(sq));

	return (NVME_SQE *)(sq->base) + sq->head;
}

void Device_CommandHandler(NVME_QID sqid, NVME_QID cqid);

#include "admin/identify_device.h"
#include "admin/get_logpage_device.h"
#include "admin/fw_download_device.h"
#include "admin/fw_commit_device.h"
#include "admin/features_device.h"

BOOL Device_DeleteIoSubmissionQueue(NVME_QID sqid, NVME_QID cqid);
BOOL Device_CreateIoSubmissionQueue(NVME_QID sqid, NVME_QID cqid);
BOOL Device_DeleteIoCompletionQueue(NVME_QID sqid, NVME_QID cqid);
BOOL Device_CreateIoCompletionQueue(NVME_QID sqid, NVME_QID cqid);

#endif	/* _NVME_DEVICE_H */

