#include <stdio.h>
#include <string.h>
#include "nvme.h"

#define	MAX_CQS	8
#define	MAX_SQS	8

#define	ADMIN_CQSIZE	16
#define	ADMIN_SQSIZE	16

CC_STATIC	NVME_CQ_ENTRY	cqAdmin[ADMIN_CQSIZE]	CC_ATTRIB_ALIGNED(4096);
CC_STATIC	NVME_SQ_ENTRY	sqAdmin[ADMIN_SQSIZE]	CC_ATTRIB_ALIGNED(4096);

NVME_QUEUE	cqs[MAX_CQS];
NVME_QUEUE	sqs[MAX_SQS];

extern	NVME_CONTROLLER	*gpController;

void Host_Init(void)
{
	memset(cqs, 0, sizeof cqs);
	NVME_QUEUE_INIT(&cqs[0], cqAdmin, ADMIN_CQSIZE);

	memset(sqs, 0, sizeof sqs);
	NVME_QUEUE_INIT(&sqs[0], sqAdmin, ADMIN_SQSIZE);

	gpController->AQA.ACQS = ADMIN_CQSIZE;
	gpController->AQA.ASQS = ADMIN_SQSIZE;
	gpController->ACQ.reg = CAST_FROM_PTR(UINT64)(cqs[0].base);
	gpController->ASQ.reg = CAST_FROM_PTR(UINT64)(sqs[0].base);
#if 0
	gpController->CSTS.reg = 0;
	gpController->CC.EN = 1;
	while (0 == gpController->CSTS.RDY) {
		sleep(1);
	}
#endif
}

CC_STATIC_ALWAYS_INLINE void updateSQT(UINT16 qid, UINT16 tail)
{
	UINT32	offset = 0x1000 + (qid * 2) * (4 << gpController->CAP.DSTRD);
	NVME_REG32_SQT	*reg = (NVME_REG32_SQT *)((UINT8 *)gpController + offset);
	reg->SQT = tail;
	WATCH(offset);
	WATCH(tail);
}

BOOL Host_RingDoorbell_SQT(UINT16 qid, UINT16 tail)
{
	ENTER();

	if (qid >= MAX_CQS)
	{
		LEAVE();
		return FALSE;
	}

	if (NULL == sqs[qid].base)
	{
		LEAVE();
		return FALSE;
	}

	updateSQT(qid, tail);
	LEAVE();
	return TRUE;
}

