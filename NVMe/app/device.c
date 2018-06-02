#include <stdlib.h>
#include <pthread.h>
#include "nvme/nvme.h"
#include "nvme_device.h"

#define	MODULE_NAME	"\t\t\t\t\t[device]"

CC_STATIC	NVME_QUEUE	*devCq = NULL;
CC_STATIC	NVME_QUEUE	*devSq = NULL;

CC_STATIC	NVME_REG64_CAP	CAP;

NVME_QUEUE *Device_GetCompletionQueue(UINT16 cqid)
{
	if (cqid >= MAX_QUEUES)
		return NULL;

	NVME_QUEUE	*cq = &devCq[cqid];
	if (!NVME_QUEUE_IS_VALID(cq))
		return NULL;

	return cq;
}

NVME_QUEUE *Device_GetSubmissionQueue(UINT16 sqid)
{
	if (sqid >= MAX_QUEUES)
		return NULL;

	NVME_QUEUE	*sq = &devSq[sqid];
	if (!NVME_QUEUE_IS_VALID(sq))
		return NULL;

	return sq;
}

void Device_InitCompletionQueue(NVME_QID cqid, void *buf, UINT16 qsize)
{
	ASSERT(cqid < MAX_QUEUES);

	NVME_QUEUE *cq = &devCq[cqid];
	ASSERT(!NVME_QUEUE_IS_VALID(cq));

	NvmeQ_Init(cq, buf, qsize);
}

void Device_InitSubmissionQueue(NVME_QID sqid, void *buf, UINT16 qsize)
{
	ASSERT(sqid < MAX_QUEUES);

	NVME_QUEUE *sq = &devSq[sqid];
	ASSERT(!NVME_QUEUE_IS_VALID(sq));

	NvmeQ_Init(sq, buf, qsize);
}

void Device_Init(void)
{
	NVME_CONTROLLER	*controller = PCIe_GetControllerRegBase(0);

	NVME_REG32_CC	CC;
	do {
		sleep(1);
		CC.reg = PCIe_ReadReg32(&controller->CC.reg);
	} while (0 == CC.EN);

	CAP.reg = PCIe_ReadReg64(&controller->CAP.reg);
	devCq = (NVME_QUEUE *)calloc(MAX_QUEUES + 1, sizeof (NVME_QUEUE));
	devSq = (NVME_QUEUE *)calloc(MAX_QUEUES + 1, sizeof (NVME_QUEUE));

	NVME_REG32_AQA	AQA;
	AQA.reg = PCIe_ReadReg32(&controller->AQA.reg);

	NVME_REG64_ASQ	ASQ;
	ASQ.reg = PCIe_ReadReg64(&controller->ASQ.reg);

	NVME_REG64_ACQ	ACQ;
	ACQ.reg = PCIe_ReadReg64(&controller->ACQ.reg);

	Device_InitCompletionQueue(NVME_CQID_ADMIN, CAST_PTR(void *)(ACQ.reg), AQA.ACQS);
	ASSERT(NULL != Device_GetCompletionQueue(NVME_CQID_ADMIN));
	Device_InitSubmissionQueue(NVME_SQID_ADMIN, CAST_PTR(void *)(ASQ.reg), AQA.ASQS);
	ASSERT(NULL != Device_GetSubmissionQueue(NVME_SQID_ADMIN));

	NVME_REG32_CSTS	CSTS;
	CSTS.reg = PCIe_ReadReg32(&controller->CSTS.reg);
	CSTS.RDY = 1;
	PCIe_WriteReg32(&controller->CSTS.reg, CSTS.reg);

	DeviceArbitration_Init();
}

BOOL Device_UpdateSQT(UINT16 sqid, UINT16 tail)
{
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	if (NULL == sq)
		return FALSE;

	if (tail > sq->size)
		return FALSE;

	sq->tail = tail;
	return TRUE;
}

BOOL Device_UpdateCQH(UINT16 cqid, UINT16 head)
{
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	if (NULL == cq)
		return FALSE;

	if (head > cq->size)
		return FALSE;

	cq->head = head;
	return TRUE;
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

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	for (;;) {
		NVME_REG32_SQT	SQT;
		SQT.reg = PCIe_ReadReg32(sqt);
		Device_UpdateSQT(sqid, SQT.SQT);

		Device_CommandHandler(sqid, cqid);

		NVME_REG32_CQH	CQH;
		CQH.reg = PCIe_ReadReg32(cqh);
		Device_UpdateCQH(cqid, CQH.CQH);

		pthread_testcancel();
	}

	LEAVE();
	return NULL;
}

