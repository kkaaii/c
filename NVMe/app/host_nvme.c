#include <string.h>
#include "nvme.h"

#define	MAX_CQS	10
#define	MAX_SQS	10

#define	ADMIN_CQSIZE	16
#define	ADMIN_SQSIZE	16

extern	NVME_CONFIG	nvmeConfig;

CC_ATTRIB_ALIGNED(4096)	NVME_CQ_ENTRY	cqAdmin[ADMIN_CQSIZE];
CC_ATTRIB_ALIGNED(4096)	NVME_SQ_ENTRY	sqAdmin[ADMIN_SQSIZE];

NVME_QUEUE	cqs[MAX_CQS];
NVME_QUEUE	sqs[MAX_SQS];

void Host_Init(void)
{
	memset(cqs, 0, sizeof cqs);
	NVME_QUEUE_INIT(&cqs[0], cqAdmin, ADMIN_CQSIZE);

	memset(sqs, 0, sizeof sqs);
	NVME_QUEUE_INIT(&sqs[0], sqAdmin, ADMIN_SQSIZE);

	nvmeConfig.AQA.ACQS = ADMIN_CQSIZE;
	nvmeConfig.AQA.ASQS = ADMIN_SQSIZE;
	nvmeConfig.ACQ.reg = CAST_FROM_PTR(UINT64)(cqs[0].base);
	nvmeConfig.ASQ.reg = CAST_FROM_PTR(UINT64)(sqs[0].base);

	nvmeConfig.CC.EN = 1;
}

#include <stdio.h>

#define	WATCH(x)	printf("%s = 0x%lx\n", #x, (UINT64)x)

NVME_CONFIG	nvmeConfig;

int main(void)
{
	Host_Init();

	WATCH(nvmeConfig.AQA.ACQS);
	WATCH(nvmeConfig.AQA.ASQS);
	WATCH(nvmeConfig.ACQ.reg);
	WATCH(nvmeConfig.ASQ.reg);

	return 0;
}

