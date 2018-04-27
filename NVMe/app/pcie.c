#include "nvme.h"

UINT8	ctrlr_regs[8192];

void *PCIe_GetControllerRegBase(UINT8 id CC_ATTRIB_UNUSED)
{
	return &ctrlr_regs[0];
}

void PCIe_WriteControllerReg32(UINT32 *reg, UINT32 val)
{
	*reg = val;
}

void PCIe_WriteControllerReg64(UINT64 *reg, UINT64 val)
{
	*reg = val;
}

UINT32 PCIe_ReadControllerReg32(UINT32 *reg)
{
	return *reg;
}

UINT64 PCIe_ReadControllerReg64(UINT64 *reg)
{
	return *reg;
}

