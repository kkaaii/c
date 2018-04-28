#include <stdio.h>
#include <stdlib.h>
#include "nvme.h"

CC_STATIC	NVME_QUEUE	*devCq = NULL;
CC_STATIC	NVME_QUEUE	*devSq = NULL;

CC_STATIC	NVME_REG64_00H	CAP;

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
	NvmeQ_Init(acq, CAST_TO_PTR(void *)(ACQ.reg), AQA.ACQS + 1);
	NvmeQ_Init(asq, CAST_TO_PTR(void *)(ASQ.reg), AQA.ASQS + 1);

	NVME_REG32_1CH	CSTS;
	CSTS.reg = PCIe_ReadControllerReg32(&controller->CSTS.reg);
	CSTS.RDY = 1;
	PCIe_WriteControllerReg32(&controller->CSTS.reg, CSTS.reg);
}

void *DeviceMain(void *context CC_ATTRIB_UNUSED)
{
	ENTER();

	Device_Init();

	LEAVE();
	return NULL;
}

