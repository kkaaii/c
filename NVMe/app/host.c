#include <string.h>
#include "nvme/nvme.h"
#include "nvme_host.h"
#include "platform.h"

#define	MODULE_NAME	"[ host ]"

#define	MAX_QUEUES	8

#define	QSIZE_ASQ	ZERO_BASED(HOST_PAGE_SIZE / sizeof (NVME_SQE))
#define	QSIZE_ACQ	ZERO_BASED(HOST_PAGE_SIZE / sizeof (NVME_CQE))

#define	QDEPTH_IOCQ	ZERO_BASED(65536)
#define	QDEPTH_IOSQ	ZERO_BASED(16384)

CC_STATIC	NVME_CQE	cqeAdmin[QSIZE_ACQ + 1]	CC_ATTRIB_ALIGNED(HOST_PAGE_SIZE);
CC_STATIC	NVME_SQE	sqeAdmin[QSIZE_ASQ + 1]	CC_ATTRIB_ALIGNED(HOST_PAGE_SIZE);

NVME_QUEUE	hostCq[MAX_QUEUES];
NVME_QUEUE	hostSq[MAX_QUEUES];

NVME_QUEUE *Host_InitCompletionQueue(NVME_QID cqid, void *buf, UINT16 qsize)
{
	ASSERT(cqid < MAX_QUEUES);
	NVME_QUEUE	*cq = &hostCq[cqid];
	ASSERT(!NVME_QUEUE_IS_VALID(cq));

	NvmeQ_Init(cq, buf, qsize);
	return cq;
}

NVME_QUEUE *Host_InitSubmissionQueue(NVME_QID sqid, void *buf, UINT16 qsize)
{
	ASSERT(sqid < MAX_QUEUES);
	NVME_QUEUE	*sq = &hostSq[sqid];
	ASSERT(!NVME_QUEUE_IS_VALID(sq));

	NvmeQ_Init(sq, buf, qsize);
	return sq;
}

NVME_QUEUE *Host_GetCompletionQueue(NVME_QID cqid)
{
	if (cqid >= MAX_QUEUES)
		return NULL;

	NVME_QUEUE *cq = &hostCq[cqid];
	if (!NVME_QUEUE_IS_VALID(cq))
		return NULL;

	return cq;
}

NVME_QUEUE *Host_GetSubmissionQueue(NVME_QID sqid)
{
	if (sqid >= MAX_QUEUES)
		return NULL;

	NVME_QUEUE *sq = &hostSq[sqid];
	if (!NVME_QUEUE_IS_VALID(sq))
		return NULL;

	return sq;
}

void Host_Init(void)
{
	memset(hostCq, 0, sizeof hostCq);
	memset(hostSq, 0, sizeof hostSq);

	NVME_QUEUE *acq = Host_InitCompletionQueue(NVME_CQID_ADMIN, cqeAdmin, QSIZE_ACQ);
	NVME_QUEUE *asq = Host_InitSubmissionQueue(NVME_SQID_ADMIN, sqeAdmin, QSIZE_ASQ);

	NVME_CONTROLLER *controller = (NVME_CONTROLLER *)PCIe_GetControllerRegBase(0);

	NVME_REG32_AQA	AQA;
	AQA.reg = 0;
	AQA.ACQS = QSIZE_ACQ;
	AQA.ASQS = QSIZE_ASQ;
	PCIe_WriteReg32(&controller->AQA.reg, AQA.reg);

	PCIe_WriteReg64(&controller->ACQ.reg, CAST_PTR(UINT64)(acq->base));
	PCIe_WriteReg64(&controller->ASQ.reg, CAST_PTR(UINT64)(asq->base));

	NVME_REG32_CSTS	CSTS;
	CSTS.reg = 0;
	PCIe_WriteReg32(&controller->CSTS.reg, CSTS.reg);

	NVME_REG32_CC	CC;
	CC.reg = PCIe_ReadReg32(&controller->CC.reg);
	CC.EN = 1;
	PCIe_WriteReg32(&controller->CC.reg, CC.reg);

	do {
		sleep(1);
		CSTS.reg = PCIe_ReadReg32(&controller->CSTS.reg);
	} while (0 == CSTS.RDY);
}

void Host_RingDoorbell_CQH(NVME_QID cqid)
{
	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG64_CAP	CAP;
	CAP.reg = PCIe_ReadReg64(&controller->CAP.reg);

	UINT32	offset = 0x1000 + (cqid * 2 + 1) * (4 << CAP.DSTRD);
	UINT32	*reg = (UINT32 *)controller + (offset >> 2);

	NVME_QUEUE *cq = Host_GetCompletionQueue(cqid);
	NVME_REG32_CQH	CQH;
	CQH.reg = 0;
	CQH.CQH = cq->head;
	PCIe_WriteReg32(reg, CQH.reg);

	HOST_DBG_MSG("(--|%02d)(--|--) @@\n", cq->head);
}

void Host_RingDoorbell_SQT(NVME_QID sqid)
{
	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG64_CAP	CAP;
	CAP.reg = PCIe_ReadReg64(&controller->CAP.reg);

	UINT32	offset = 0x1000 + (sqid * 2) * (4 << CAP.DSTRD);
	UINT32	*reg = (UINT32 *)controller + (offset >> 2);

	NVME_QUEUE *sq = Host_GetSubmissionQueue(sqid);
	NVME_REG32_SQT	SQT;
	SQT.reg = 0;
	SQT.SQT = sq->tail;
	PCIe_WriteReg32(reg, SQT.reg);

	HOST_DBG_MSG("(--|%02d)(--|--) @@\n", sq->tail);
}

NVME_CQE *Host_CheckResponse(NVME_QUEUE *cq)
{
	static BOOL	phase = FALSE;

	ASSERT(NULL != cq);

	NVME_CQE	*cqe = (NVME_CQE *)(cq->base) + cq->tail;
	if (cqe->dw3.P == phase)
		return NULL;

	NVME_QUEUE_INC_TAIL(cq);
	NVME_QUEUE_INC_HEAD(cq);
	if (0 == cq->head)
		phase = !phase;

	UINT16	sqid = cqe->dw2.SQID;
	NVME_QUEUE	*sq = Host_GetSubmissionQueue(sqid);
	if (NULL != sq && !NVME_QUEUE_IS_EMPTY(sq)) {
		NVME_QUEUE_INC_HEAD(sq);
	}

	HOST_DBG_MSG("(%02d|%02d)(%02d|%02d) %04xh %03xh\n",
		sq->head, sq->tail, cq->head, cq->tail, cqe->dw3.CID, cqe->dw3.SF);

	return cqe;
}

BOOL Host_SyncAdminCommand(NVME_CID cid)
{
	NVME_STATUS	status;

	Host_RingDoorbell_SQT(NVME_SQID_ADMIN);
	status = Host_WaitForCompletion(NVME_CQID_ADMIN, cid)->dw3.SF;
	Host_RingDoorbell_CQH(NVME_CQID_ADMIN);

	return (eSF_SuccessfulCompletion == status);
}

void *HostMain(void *context CC_ATTRIB_UNUSED)
{
	ENTER();
	Host_Init();

	NVME_QID	sqid = NVME_SQID_ADMIN;
	NVME_QID	cqid = NVME_CQID_ADMIN;
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(sqid);
	NVME_QUEUE	*acq = Host_GetCompletionQueue(cqid);

	NVME_CID	cid;

	cid = Host_SetFeatures(NVME_NSID_NONE, eFID_Arbitration, 3, FALSE, NULL, 0);
	if (!Host_SyncAdminCommand(cid)) {
		return NULL;
	}

	UINT8	*buf = Platform_MemAlign(HOST_PAGE_SIZE, HOST_PAGE_SIZE * 2);
	Host_InitCompletionQueue(1, buf, ZERO_BASED(HOST_PAGE_SIZE / sizeof (NVME_CQE)));
	cid = Host_CreateIoCompletionQueue(NVME_NSID_NONE, 1, buf, HOST_PAGE_SIZE);
	if (!Host_SyncAdminCommand(cid)) {
		free(buf);
		return NULL;
	}

	buf += HOST_PAGE_SIZE;
	Host_InitSubmissionQueue(1, buf, ZERO_BASED(HOST_PAGE_SIZE / sizeof (NVME_SQE)));
	cid = Host_CreateIoSubmissionQueue(NVME_NSID_NONE, 1, 1, buf, HOST_PAGE_SIZE);
	if (!Host_SyncAdminCommand(cid)) {
		free(buf - HOST_PAGE_SIZE);
		return NULL;
	}
	
	ASSERT(0 == HostTest_IdentifyParameters());
	ASSERT(0 == HostTest_GetLogPageParameters());
	ASSERT(0 == HostTest_FwDownloadParameters());
	ASSERT(0 == HostTest_FwCommitParameters());
	ASSERT(0 == HostTest_GetFeaturesParameters());
	ASSERT(0 == HostTest_SetFeaturesParameters());
	ASSERT(0 == HostTest_CreateIoCompletionQueueParameters());
	ASSERT(0 == HostTest_CreateIoSubmissionQueueParameters());
	ASSERT(0 == HostTest_DeleteIoSubmissionQueueParameters());
	ASSERT(0 == HostTest_DeleteIoCompletionQueueParameters());

#if 0
	for (;;) {
		sleep(1);

		if (!NVME_QUEUE_IS_FULL(asq))
			Host_IssueCommand(asq);

		if (0 == (rand() & 3))
			Host_RingDoorbell_SQT(sqid);

		Host_CheckResponse(acq);

		if (0 == (rand() & 3))
			Host_RingDoorbell_CQH(cqid);
	}
#endif

	cid = Host_DeleteIoSubmissionQueue(NVME_NSID_NONE, 1);
	Host_SyncAdminCommand(cid);

	cid = Host_DeleteIoCompletionQueue(NVME_NSID_NONE, 1);
	Host_SyncAdminCommand(cid);

	LEAVE();
	free(buf - HOST_PAGE_SIZE);
	return NULL;
}

