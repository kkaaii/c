#include <stdio.h>
#include <string.h>
#include "nvme.h"

#define	MAX_QUEUES	8

#define	QDEPTH_ADMIN	16

CC_STATIC	NVME_CQ_ENTRY	cqAdmin[QDEPTH_ADMIN]	CC_ATTRIB_ALIGNED(4096);
CC_STATIC	NVME_SQ_ENTRY	sqAdmin[QDEPTH_ADMIN]	CC_ATTRIB_ALIGNED(4096);

NVME_QUEUE	hostCq[MAX_QUEUES];
NVME_QUEUE	hostSq[MAX_QUEUES];

void Host_Init(void)
{
	memset(hostCq, 0, sizeof hostCq);
	memset(hostSq, 0, sizeof hostSq);

	NVME_QUEUE	*acq = &hostCq[NVME_CQID_ADMIN];
	NVME_QUEUE	*asq = &hostSq[NVME_SQID_ADMIN];
	NvmeQ_Init(acq, cqAdmin, QDEPTH_ADMIN);
	NvmeQ_Init(asq, sqAdmin, QDEPTH_ADMIN);

	NVME_CONTROLLER *controller = (NVME_CONTROLLER *)PCIe_GetControllerRegBase(0);

	NVME_REG32_24H	AQA;
	AQA.reg = 0;
	AQA.ACQS = QDEPTH_ADMIN - 1;
	AQA.ASQS = QDEPTH_ADMIN - 1;
	PCIe_WriteControllerReg32(&controller->AQA.reg, AQA.reg);

	PCIe_WriteControllerReg64(&controller->ACQ.reg, CAST_FROM_PTR(UINT64)(acq->base));
	PCIe_WriteControllerReg64(&controller->ASQ.reg, CAST_FROM_PTR(UINT64)(asq->base));

	NVME_REG32_1CH	CSTS;
	CSTS.reg = 0;
	PCIe_WriteControllerReg32(&controller->CSTS.reg, CSTS.reg);

	NVME_REG32_14H	CC;
	CC.reg = PCIe_ReadControllerReg32(&controller->CC.reg);
	CC.EN = 1;
	PCIe_WriteControllerReg32(&controller->CC.reg, CC.reg);

	do {
		sleep(1);
		CSTS.reg = PCIe_ReadControllerReg32(&controller->CSTS.reg);
	} while (0 == CSTS.RDY);
}

CC_STATIC NVME_QUEUE *Host_GetSq(UINT16 sqid)
{
	if (sqid > MAX_QUEUES)
		return NULL;

	NVME_QUEUE *sq = &hostSq[sqid];
	if (!NVME_QUEUE_IS_VALID(sq))
		return NULL;

	return sq;
}

BOOL Host_IssueCommand(UINT16 sqid)
{
	NVME_QUEUE *sq = Host_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	if (NVME_QUEUE_IS_FULL(sq))
		return FALSE;

	NVME_QUEUE_INC_TAIL(sq);
	return TRUE;
}

BOOL Host_RingDoorbell_SQT(UINT16 sqid)
{
	ENTER();

	NVME_QUEUE *sq = Host_GetSq(sqid);
	if (NULL == sq)
		return FALSE;

	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG64_00H	CAP;
	CAP.reg = PCIe_ReadControllerReg64(&controller->CAP.reg);

	UINT32	offset = 0x1000 + (sqid * 2) * (4 << CAP.DSTRD);
	UINT32	*reg = (UINT32 *)controller + (offset >> 2);

	NVME_REG32_SQT	SQT;
	SQT.reg = 0;
	SQT.SQT = sq->tail;
	PCIe_WriteControllerReg32(reg, SQT.reg);

	LEAVE();
	return TRUE;
}

