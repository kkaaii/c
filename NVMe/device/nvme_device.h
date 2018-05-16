#ifndef	_NVME_DEVICE_H
#define	_NVME_DEVICE_H

#define	MODULE_NAME	"\t\t\t\t\t[device]"

#define	DEV_DBG_MSG(...)	DBG_MSG(MODULE_NAME __VA_ARGS__)

typedef enum {
	eDeviceState_FetchCommand,
	eDeviceState_Identify,
	eDeviceState_GetLogPage,
	eDeviceState_FwDownload,
	eDeviceState_FwCommit,
	eDeviceState_ReturnStatus
} EDeviceState;

NVME_QUEUE *Device_GetCompletionQueue(NVME_QID cqid);
NVME_QUEUE *Device_GetSubmissionQueue(NVME_QID sqid);

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

#include "identify_device.h"
#include "get_logpage_device.h"
#include "fw_download_device.h"
#include "fw_commit_device.h"

#endif	/* _NVME_DEVICE_H */

