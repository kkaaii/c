#include <string.h>
#include "nvme/nvme.h"
#include "nvme_host.h"
#include "platform.h"

#define	MODULE_NAME	"[ host ]"

#define	MAX_QUEUES	8

#define	QDEPTH_ADMIN	ZERO_BASED(16)
#define	QDEPTH_IOCQ	ZERO_BASED(65536)
#define	QDEPTH_IOSQ	ZERO_BASED(16384)

CC_STATIC	NVME_CQE	cqAdmin[QDEPTH_ADMIN + 1]	CC_ATTRIB_ALIGNED(4096);
CC_STATIC	NVME_SQE	sqAdmin[QDEPTH_ADMIN + 1]	CC_ATTRIB_ALIGNED(4096);

NVME_QUEUE	hostCq[MAX_QUEUES];
NVME_QUEUE	hostSq[MAX_QUEUES];

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

	NVME_QUEUE	*acq = &hostCq[NVME_CQID_ADMIN];
	NVME_QUEUE	*asq = &hostSq[NVME_SQID_ADMIN];
	NvmeQ_Init(acq, cqAdmin, QDEPTH_ADMIN);
	NvmeQ_Init(asq, sqAdmin, QDEPTH_ADMIN);

	NVME_CONTROLLER *controller = (NVME_CONTROLLER *)PCIe_GetControllerRegBase(0);

	NVME_REG32_AQA	AQA;
	AQA.reg = 0;
	AQA.ACQS = QDEPTH_ADMIN;
	AQA.ASQS = QDEPTH_ADMIN;
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
/*
NVME_STATUS Host_GetStatus(NVME_QUEUE *cq, NVME_CID cid)
{
	NVME_CQE	*cqe;

	do {
		cqe = Host_CheckResponse(cq);
	} while(NULL == cqe || cid != cqe->dw3.CID);

	return cqe->dw3.SF;
}
*/
void *HostMain(void *context CC_ATTRIB_UNUSED)
{
	ENTER();
	Host_Init();

	NVME_QID	sqid = NVME_SQID_ADMIN;
	NVME_QID	cqid = NVME_CQID_ADMIN;
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(sqid);
	NVME_QUEUE	*acq = Host_GetCompletionQueue(cqid);

	{
#if 0
		UINT32	bytes;
		void	*buf;

		bytes = (QDEPTH_IOCQ + 1) * sizeof (NVME_CQE);
		buf = malloc_align(HOST_PAGE_SIZE, bytes);

		NVME_CID	cid;
		NVME_QID	cqid = 1;
		NvmeQ_Init(&hostCq[cqid], buf, QDEPTH_IOCQ);
		cid = Host_CreateIoCq(asq, cqid, buf, bytes);
		Host_RingDoorbell_SQT(NVME_SQID_ADMIN);
		ASSERT(eSF_SuccessfulCompletion == Host_WaitForCompletion(cqid, cid));

		bytes = (QDEPTH_IOSQ + 1) * sizeof (NVME_SQE);
		buf = malloc_align(HOST_PAGE_SIZE, bytes);

		NVME_QID	sqid = 1;
		NvmeQ_Init(&hostCq[sqid], buf, QDEPTH_IOSQ);
		cid = Host_CreateIoSq(asq, sqid, buf, bytes);
		Host_RingDoorbell_SQT(NVME_SQID_ADMIN);
		ASSERT(eSF_SuccessfulCompletion == Host_WaitForCompletion(cqid, cid));
#endif
		HostTest_IdentifyParameters();
		HostTest_GetLogPageParameters();
		HostTest_FwDownloadParameters();
		HostTest_FwCommitParameters();
		HostTest_GetFeaturesParameters();
		HostTest_SetFeaturesParameters();
	}

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
	LEAVE();
	return NULL;
}

