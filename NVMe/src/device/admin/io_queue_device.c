#include "nvme/nvme.h"
#include "nvme_device.h"

struct {
	UINT32  ref_count;
} completionQueues[MAX_QUEUES] = {
	[NVME_CQID_ADMIN] = 1
};

BOOL Device_CreateIoCompletionQueue(NVME_QID sqid, NVME_QID cqid)
{
	ASSERT(NVME_SQID_ADMIN == sqid);
	ASSERT(NVME_CQID_ADMIN == cqid);

	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*asq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*acq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != asq && NULL != acq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(asq);
	NVME_QID	qid = sqe->CDW10.createIoCompletionQueue.QID;
	UINT16		qsize = sqe->CDW10.createIoCompletionQueue.QSIZE;

	do {
		if (NVME_NSID_NONE != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			break;
		}

		if (qid >= MAX_QUEUES || NULL != Device_GetCompletionQueue(qid)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueIdentifier;
			break;
		}

		if (qsize > ZERO_BASED(MQES)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueSize;
			break;
		}

		if (0 == sqe->CDW11.createIoCompletionQueue.PC) {
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			break;
		}

		Device_InitCompletionQueue(qid, CAST_PTR(void *)(sqe->DPTR.PRP1), qsize);
	} while (FALSE);

	DEV_DBG_MSG("+++ Create I/O Completion Queue\n");
	DEV_DBG_MSG("    QID \t= %04Xh\n", qid);
	DEV_DBG_MSG("    QSIZE\t= %04Xh\n", qsize);
	DEV_DBG_MSG("--- Status\t= %04Xh\n", status);

	Device_SetNvmeStatus(acq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

BOOL Device_DeleteIoCompletionQueue(NVME_QID sqid, NVME_QID cqid)
{
	ASSERT(NVME_SQID_ADMIN == sqid);
	ASSERT(NVME_CQID_ADMIN == cqid);

	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*asq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*acq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != asq && NULL != acq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(asq);
	NVME_QID	qid = sqe->CDW10.deleteIoCompletionQueue.QID;

	do {
		if (NVME_NSID_NONE != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			break;
		}

		if (qid >= MAX_QUEUES ||
			NVME_CQID_ADMIN == qid ||
			NULL == Device_GetCompletionQueue(qid)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueIdentifier;
			break;
		}

		if (0 != completionQueues[qid].ref_count) {
			status = eSF_DoNotRetry | eSF_InvalidQueueDeletion;
			break;
		}

		NVME_QUEUE      *queue = Device_GetCompletionQueue(qid);
		ASSERT(NULL != queue);
		queue->base = NULL;
	} while (FALSE);

	DEV_DBG_MSG("+++ Delete I/O Completion Queue\n");
	DEV_DBG_MSG("    QID \t= %04Xh\n", qid);
	DEV_DBG_MSG("--- Status\t= %04Xh\n", status);

	Device_SetNvmeStatus(acq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

BOOL Device_CreateIoSubmissionQueue(NVME_QID sqid, NVME_QID cqid)
{
	ASSERT(NVME_SQID_ADMIN == sqid);
	ASSERT(NVME_CQID_ADMIN == cqid);

	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*asq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*acq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != asq && NULL != acq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(asq);
	NVME_QID	qid = sqe->CDW10.createIoSubmissionQueue.QID;
	UINT16		qsize = sqe->CDW10.createIoSubmissionQueue.QSIZE;

	cqid = sqe->CDW11.createIoSubmissionQueue.CQID;
	do {
		if (NVME_NSID_NONE != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			break;
		}

		if (qid >= MAX_QUEUES || NULL != Device_GetSubmissionQueue(qid)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueIdentifier;
			break;
		}

		if (qsize > ZERO_BASED(MQES)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueSize;
			break;
		}

		if (NULL == Device_GetCompletionQueue(cqid)) {
			status = eSF_DoNotRetry | eSF_CompletionQueueInvalid;
			break;
		}

		if (!DeviceArbitration_AddQueuePair(qid, cqid)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueIdentifier;
			break;
		}

		if (0 == sqe->CDW11.createIoSubmissionQueue.PC) {
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			break;
		}

		Device_InitSubmissionQueue(qid, CAST_PTR(void *)(sqe->DPTR.PRP1), qsize);
		++completionQueues[cqid].ref_count;
	} while (FALSE);

	DEV_DBG_MSG("+++ Create I/O Submission Queue\n");
	DEV_DBG_MSG("    QID \t= %04Xh\n", qid);
	DEV_DBG_MSG("    QSIZE\t= %04Xh\n", qsize);
	DEV_DBG_MSG("    CQID\t= %04Xh\n", cqid);
	DEV_DBG_MSG("--- Status\t= %04Xh\n", status);

	Device_SetNvmeStatus(acq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

BOOL Device_DeleteIoSubmissionQueue(NVME_QID sqid, NVME_QID cqid)
{
	ASSERT(NVME_SQID_ADMIN == sqid);
	ASSERT(NVME_CQID_ADMIN == cqid);

	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*asq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*acq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != asq && NULL != acq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(asq);
	NVME_QID	qid = sqe->CDW10.deleteIoSubmissionQueue.QID;

	do {
		if (NVME_NSID_NONE != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			break;
		}

		if (qid >= MAX_QUEUES ||
			NVME_SQID_ADMIN == qid ||
			NULL == Device_GetSubmissionQueue(qid)) {
			status = eSF_DoNotRetry | eSF_InvalidQueueIdentifier;
			break;
		}

		cqid = DeviceArbitration_DelQueuePair(qid);
		ASSERT(0 < completionQueues[cqid].ref_count);
		--completionQueues[cqid].ref_count;

		NVME_QUEUE *queue = Device_GetSubmissionQueue(qid);
		ASSERT(NULL != queue);
		queue->base = NULL;
	} while (FALSE);

	DEV_DBG_MSG("+++ Delete I/O Submission Queue\n");
	DEV_DBG_MSG("    QID \t= %04Xh\n", qid);
	DEV_DBG_MSG("--- Status\t= %04Xh\n", status);

	Device_SetNvmeStatus(acq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

