#include <stdio.h>
#include <stdlib.h>
#include "nvme.h"

CC_STATIC	NVME_QUEUE	*devCq = NULL;
CC_STATIC	NVME_QUEUE	*devSq = NULL;

CC_STATIC	NVME_REG64_00H	CAP;

CC_STATIC_ALWAYS_INLINE
NVME_QUEUE *Device_GetCq(UINT16 cqid)
{
	if (cqid > CAP.MQES)
		return NULL;

	NVME_QUEUE	*cq = &devCq[cqid];
	if (!NVME_QUEUE_IS_VALID(cq))
		return NULL;

	return cq;
}

CC_STATIC_ALWAYS_INLINE
NVME_QUEUE *Device_GetSq(UINT16 sqid)
{
	if (sqid > CAP.MQES)
		return NULL;

	NVME_QUEUE	*sq = &devSq[sqid];
	if (!NVME_QUEUE_IS_VALID(sq))
		return NULL;

	return sq;
}

void Device_Init(void)
{
	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG32_14H	CC;
	do {
		sleep(1);
		CC.reg = PCIe_ReadControllerReg32(&controller->CC.reg);
	} while (0 == CC.EN);

	CAP.reg = PCIe_ReadControllerReg64(&controller->CAP.reg);
	devCq = (NVME_QUEUE *)calloc(CAP.MQES + 1, sizeof (NVME_QUEUE));
	devSq = (NVME_QUEUE *)calloc(CAP.MQES + 1, sizeof (NVME_QUEUE));

	NVME_REG32_24H	AQA;
	AQA.reg = PCIe_ReadControllerReg32(&controller->AQA.reg);

	NVME_REG64_28H	ASQ;
	ASQ.reg = PCIe_ReadControllerReg64(&controller->ASQ.reg);

	NVME_REG64_30H	ACQ;
	ACQ.reg = PCIe_ReadControllerReg64(&controller->ACQ.reg);

	NVME_QUEUE *acq = &devCq[NVME_CQID_ADMIN];
	NVME_QUEUE *asq = &devSq[NVME_SQID_ADMIN];
	NvmeQ_Init(acq, CAST_PTR(void *)(ACQ.reg), AQA.ACQS + 1);
	NvmeQ_Init(asq, CAST_PTR(void *)(ASQ.reg), AQA.ASQS + 1);

	NVME_REG32_1CH	CSTS;
	CSTS.reg = PCIe_ReadControllerReg32(&controller->CSTS.reg);
	CSTS.RDY = 1;
	PCIe_WriteControllerReg32(&controller->CSTS.reg, CSTS.reg);
}

BOOL Device_UpdateSQT(UINT16 sqid, UINT16 tail)
{
	NVME_QUEUE	*sq = Device_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	if (tail > sq->size)
		return FALSE;

	sq->tail = tail;
	return TRUE;
}

BOOL Device_UpdateCQH(UINT16 cqid, UINT16 head)
{
	NVME_QUEUE	*cq = Device_GetCq(cqid);
	if (NULL == cq)
		return FALSE;

	if (head > cq->size)
		return FALSE;

	cq->head = head;
	return TRUE;
}

BOOL Device_HandleCommand(UINT16 sqid)
{
	NVME_QUEUE	*sq = Device_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	if (NVME_QUEUE_IS_EMPTY(sq))
		return FALSE;

	NVME_QUEUE	*cq = Device_GetCq(NVME_CQID_ADMIN);
	if (NVME_QUEUE_IS_FULL(cq))
		return FALSE;

	NVME_SQE	*sqe = (NVME_SQE *)(sq->base) + sq->head;
	UINT16		cid = sqe->CDW0.CID;

	NVME_CQE	*cqe = (NVME_CQE *)(cq->base) + cq->tail;
	cqe->dw2.SQID	= sqid;
	cqe->dw2.SQHD	= sq->head;
	cqe->dw3.CID	= cid;
	cqe->dw3.P	= !cqe->dw3.P;

	NVME_QUEUE_INC_HEAD(sq);
	NVME_QUEUE_INC_TAIL(cq);
}

void *DeviceMain(void *context CC_ATTRIB_UNUSED)
{
	UINT16	sqid = NVME_SQID_ADMIN;
	UINT16	cqid = NVME_CQID_ADMIN;

	ENTER();

	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	Device_Init();

	UINT32	offset = 0x1000 + (sqid * 2) * (4 << CAP.DSTRD);
	UINT32	*sqt = (UINT32 *)controller + (offset >> 2);

	offset += (4 << CAP.DSTRD);
	UINT32	*cqh = (UINT32 *)controller + (offset >> 2);

	for (;;) {
		NVME_REG32_SQT	SQT;
		SQT.reg = PCIe_ReadControllerReg32(sqt);
		Device_UpdateSQT(sqid, SQT.SQT);

		Device_HandleCommand(sqid);

		NVME_REG32_CQH	CQH;
		CQH.reg = PCIe_ReadControllerReg32(cqh);
		Device_UpdateCQH(cqid, CQH.CQH);
	}

	LEAVE();
	return NULL;
}

