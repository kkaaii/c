#include "nvme/nvme.h"
#include "nvme_device.h"

typedef BOOL (*StateHandler)(NVME_QID sqid, NVME_QID cqid);

CC_STATIC	BOOL Device_FetchCommand(NVME_QID sqid, NVME_QID cqid);
CC_STATIC	BOOL Device_ReturnStatus(NVME_QID sqid, NVME_QID cqid);

CC_STATIC	EDeviceState	gState = eDeviceState_FetchCommand;

CC_STATIC	StateHandler	gHandlers[] = {
	[eDeviceState_FetchCommand]	= Device_FetchCommand,
	[eDeviceState_DeleteIoSq]	= Device_DeleteIoSubmissionQueue,
	[eDeviceState_CreateIoSq]	= Device_CreateIoSubmissionQueue,
	[eDeviceState_GetLogPage]	= Device_GetLogPage,
	[eDeviceState_DeleteIoCq]	= Device_DeleteIoCompletionQueue,
	[eDeviceState_CreateIoCq]	= Device_CreateIoCompletionQueue,
	[eDeviceState_Identify]		= Device_Identify,
	[eDeviceState_FwDownload]	= Device_FwDownload,
	[eDeviceState_FwCommit]		= Device_FwCommit,
	[eDeviceState_GetFeatures]	= Device_GetFeatures,
	[eDeviceState_SetFeatures]	= Device_SetFeatures,
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
	static const struct {
		UINT8           opcode;
		EDeviceState    state;
	} table[] = {
		{NVME_OPC_ADMIN_DELETE_IOSQ,	eDeviceState_DeleteIoSq},
		{NVME_OPC_ADMIN_CREATE_IOSQ,	eDeviceState_CreateIoSq},
		{NVME_OPC_ADMIN_GET_LOG_PAGE,   eDeviceState_GetLogPage},
		{NVME_OPC_ADMIN_DELETE_IOCQ,	eDeviceState_DeleteIoCq},
		{NVME_OPC_ADMIN_CREATE_IOCQ,	eDeviceState_CreateIoCq},
		{NVME_OPC_ADMIN_IDENTIFY,       eDeviceState_Identify},
		{NVME_OPC_ADMIN_GET_FEATURES,   eDeviceState_GetFeatures},
		{NVME_OPC_ADMIN_SET_FEATURES,   eDeviceState_SetFeatures},
		{NVME_OPC_ADMIN_FW_DOWNLOAD,    eDeviceState_FwDownload},
		{NVME_OPC_ADMIN_FW_COMMIT,      eDeviceState_FwCommit}
	};

	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);
	UINT32		i;

	if (NVME_QUEUE_IS_EMPTY(sq))
		return FALSE;

	if (NVME_QUEUE_IS_FULL(cq))
		return FALSE;

	Device_SetNvmeStatus(cq, eSF_SuccessfulCompletion);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);
	for (i = 0; i < sizeof (table) / sizeof (table[0]); ++i) {
		if (sqe->CDW0.OPC == table[i].opcode) {
			Device_ChangeState(table[i].state);
			return TRUE;
		}
	}

	Device_SetNvmeStatus(cq, eSF_InvalidCommandOpcode);
	Device_ChangeState(eDeviceState_ReturnStatus);
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

