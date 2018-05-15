#include "nvme.h"
#include "nvme_device.h"

typedef BOOL (*StateHandler)(NVME_QID sqid, NVME_QID cqid);

CC_STATIC	BOOL Device_FetchCommand(NVME_QID sqid, NVME_QID cqid);
CC_STATIC	BOOL Device_ReturnStatus(NVME_QID sqid, NVME_QID cqid);

CC_STATIC	EDeviceState	gState = eDeviceState_FetchCommand;

CC_STATIC	StateHandler	gHandlers[] = {
	[eDeviceState_FetchCommand]	= Device_FetchCommand,
	[eDeviceState_Identify]		= Device_Identify,
	[eDeviceState_GetLogPage]	= Device_GetLogPage,
	[eDeviceState_ReturnStatus]	= Device_ReturnStatus,
};

void Device_CommandHandler(NVME_QID sqid, NVME_QID cqid)
{
	StateHandler	handler;

	do {
		handler = gHandlers[gState];
	} while (NULL != handler && (*handler)(sqid, cqid));
}

void Device_ChangeState(EDeviceState state)
{
	DEV_DBG_MSG("%u -> %u\n", gState, state);
	gState = state;
}

void Device_SetNvmeStatus(NVME_QUEUE *cq, NVME_STATUS status)
{
	NVME_CQE	*cqe = Device_GetCompletionQueueEntry(cq);
	cqe->dw3.SF = status;
}

BOOL Device_FetchCommand(NVME_QID sqid, NVME_QID cqid)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	if (NVME_QUEUE_IS_EMPTY(sq))
		return FALSE;

	if (NVME_QUEUE_IS_FULL(cq))
		return FALSE;

	Device_SetNvmeStatus(cq, NVME_STATUS_SUCCESSFUL_COMPLETION);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);
	switch (sqe->CDW0.OPC) {
	case NVME_OPC_ADMIN_IDENTIFY:
		Device_ChangeState(eDeviceState_Identify);
		break;

	case NVME_OPC_ADMIN_GET_LOG_PAGE:
		Device_ChangeState(eDeviceState_GetLogPage);
		break;

	default:
		Device_SetNvmeStatus(cq, NVME_STATUS_INVALID_COMMAND_OPCODE);
		Device_ChangeState(eDeviceState_ReturnStatus);
		break;
	}

	return TRUE;
}

BOOL Device_ReturnStatus(NVME_QID sqid, NVME_QID cqid)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);
	NVME_CQE	*cqe = Device_GetCompletionQueueEntry(cq);
	NVME_CID	cid = sqe->CDW0.CID;

	cqe->dw2.SQID	= sqid;
	cqe->dw2.SQHD	= sq->head;

	cqe->dw3.CID	= cid;
	cqe->dw3.P	^= 1;

	NVME_QUEUE_INC_HEAD(sq);
	NVME_QUEUE_INC_TAIL(cq);

	DEV_DBG_MSG("(%02d|--)(--|%02d) %04Xh %01X\n",
		sq->head, cq->tail, cid, cqe->dw3.P);

	Device_ChangeState(eDeviceState_FetchCommand);
	return TRUE;
}

