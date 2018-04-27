#include <stdio.h>
#include "nvme.h"

CC_STATIC	NVME_CONTROLLER	*gpController;

void *DeviceMain(void *context CC_ATTRIB_UNUSED)
{
	NVME_REG32_14H	cc;
	NVME_REG32_1CH	csts;

	ENTER();
	gpController = (NVME_CONTROLLER *)PCIe_GetControllerRegBase(0);
	do {
		sleep(1);
		cc.reg = PCIe_ReadControllerReg32(&gpController->CC.reg);
	} while (0 == cc.EN);

	csts.reg = PCIe_ReadControllerReg32(&gpController->CSTS.reg);
	csts.RDY = 1;
	PCIe_WriteControllerReg32(&gpController->CSTS.reg, csts.reg);

	LEAVE();
	return NULL;
}

