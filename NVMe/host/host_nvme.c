#include <string.h>
#include "nvme.h"

#define	HOST_DBG_MSG(...)	DBG_MSG("[ host ]" __VA_ARGS__)

#define	MAX_QUEUES	8

#define	QDEPTH_ADMIN	16

CC_STATIC	NVME_CQE	cqAdmin[QDEPTH_ADMIN]	CC_ATTRIB_ALIGNED(4096);
CC_STATIC	NVME_SQE	sqAdmin[QDEPTH_ADMIN]	CC_ATTRIB_ALIGNED(4096);

NVME_QUEUE	hostCq[MAX_QUEUES];
NVME_QUEUE	hostSq[MAX_QUEUES];

CC_STATIC_ALWAYS_INLINE
NVME_QUEUE *Host_GetCq(UINT16 cqid)
{
	if (cqid >= MAX_QUEUES)
		return NULL;

	NVME_QUEUE *cq = &hostCq[cqid];
	if (!NVME_QUEUE_IS_VALID(cq))
		return NULL;

	return cq;
}

CC_STATIC_ALWAYS_INLINE
NVME_QUEUE *Host_GetSq(UINT16 sqid)
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
	AQA.ACQS = QDEPTH_ADMIN - 1;
	AQA.ASQS = QDEPTH_ADMIN - 1;
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

NVME_CID Host_AllocCommandId(void)
{
	static NVME_CID	cid = 0;
	return cid++;
}

BOOL Host_IssueCommand(UINT16 sqid)
{
	NVME_QUEUE *sq = Host_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	if (NVME_QUEUE_IS_FULL(sq))
		return FALSE;

	NVME_CID	cid = Host_AllocCommandId();
	NVME_SQE	*sqe = (NVME_SQE *)(sq->base) + sq->tail;
	sqe->CDW0.CID = cid;
	NVME_QUEUE_INC_TAIL(sq);

	HOST_DBG_MSG("(%02d|%02d)(--|--) 0x%04x\n", sq->head, sq->tail, cid);
	return TRUE;
}

BOOL Host_RingDoorbell_SQT(UINT16 sqid)
{
	NVME_QUEUE *sq = Host_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG64_CAP	CAP;
	CAP.reg = PCIe_ReadReg64(&controller->CAP.reg);

	UINT32	offset = 0x1000 + (sqid * 2) * (4 << CAP.DSTRD);
	UINT32	*reg = (UINT32 *)controller + (offset >> 2);

	NVME_REG32_SQT	SQT;
	SQT.reg = 0;
	SQT.SQT = sq->tail;
	PCIe_WriteReg32(reg, SQT.reg);

	HOST_DBG_MSG("(--|%02d)(--|--) @@@@@@\n", sq->tail);
	return TRUE;
}

BOOL Host_CheckResponse(UINT16 cqid)
{
	static BOOL	phase = FALSE;

	NVME_QUEUE	*cq = Host_GetCq(cqid);
	if (NULL == cq)
		return FALSE;

	NVME_CQE	*cqe = (NVME_CQE *)(cq->base) + cq->tail;
	if (cqe->dw3.P == phase)
		return FALSE;

	UINT16	sqid = cqe->dw2.SQID;
	NVME_QUEUE	*sq = Host_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	NVME_QUEUE_INC_TAIL(cq);
	NVME_QUEUE_INC_HEAD(cq);
	if (0 == cq->head)
		phase = !phase;

	NVME_QUEUE_INC_HEAD(sq);

	HOST_DBG_MSG("(%02d|%02d)(%02d|%02d) 0x%04x\n",
		sq->head, sq->tail, cq->head, cq->tail, cqe->dw3.CID);
	return TRUE;
}

BOOL Host_RingDoorbell_CQH(UINT16 cqid)
{
	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG64_CAP	CAP;
	CAP.reg = PCIe_ReadReg64(&controller->CAP.reg);

	UINT32	offset = 0x1000 + (cqid * 2 + 1) * (4 << CAP.DSTRD);
	UINT32	*reg = (UINT32 *)controller + (offset >> 2);

	NVME_QUEUE	*cq = Host_GetCq(cqid);
	NVME_REG32_CQH	CQH;
	CQH.reg = 0;
	CQH.CQH = cq->head;
	PCIe_WriteReg32(reg, CQH.reg);

	HOST_DBG_MSG("(--|--)(%02d|--) @@@@@@\n", cq->head);
}

