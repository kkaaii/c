#include "nvme/nvme.h"
#include "nvme_host.h"
#include "nvme_device.h"

static struct {
	NVME_NSID	nsid;
	NVME_FID	fid;
	UINT8		sel;
	NVME_STATUS	status;
} testcases[] = {
	/* Case 01: Get Features w/SEL = 000 (Current); NSID = 00000000h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_Arbitration,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_PowerManagement,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 02: Get Features w/SEL = 001 (Default); NSID = 00000000h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_Arbitration,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_PowerManagement,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 03: Get Features w/SEL = 010 (Saved); NSID = 00000000h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_Arbitration,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_PowerManagement,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 04: Get Features w/SEL = 011 (SPT CAP); NSID = 00000000h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_Arbitration,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_PowerManagement,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 05: Get Features w/SEL = 000 (Current); NSID = FFFFFFFFh */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_Arbitration,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_PowerManagement,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_ErrorRecovery,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_NumberOfQueues,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 06: Get Features w/SEL = 001 (Default); NSID = FFFFFFFFh */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_Arbitration,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_PowerManagement,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_ErrorRecovery,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_NumberOfQueues,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 07: Get Features w/SEL = 010 (Saved); NSID = FFFFFFFFh */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_Arbitration,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_PowerManagement,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_ErrorRecovery,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_NumberOfQueues,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 08: Get Features w/SEL = 011 (SPT CAP); NSID = FFFFFFFFh */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_Arbitration,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_PowerManagement,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_ErrorRecovery,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_NumberOfQueues,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 09: Get Features w/SEL = 000 (Current); NSID = 00000001h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_Arbitration,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_PowerManagement,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 0,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 10: Get Features w/SEL = 001 (Default); NSID = 00000001h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_Arbitration,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_PowerManagement,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 1,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 11: Get Features w/SEL = 010 (Saved); NSID = 00000001h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_Arbitration,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_PowerManagement,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 2,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 12: Get Features w/SEL = 011 (SPT CAP); NSID = 00000001h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_Arbitration,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_PowerManagement,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_ErrorRecovery,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_NumberOfQueues,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 3,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 13: Get Features w/SEL = 000 (Current); NSID = 00000002h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_Arbitration,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_PowerManagement,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_ErrorRecovery,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_NumberOfQueues,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 0,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Case 14: Get Features w/SEL = 001 (Default); NSID = 00000002h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_Arbitration,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_PowerManagement,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_ErrorRecovery,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_NumberOfQueues,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 1,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Case 15: Get Features w/SEL = 010 (Saved); NSID = 00000002h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_Arbitration,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_PowerManagement,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_ErrorRecovery,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_NumberOfQueues,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 2,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Case 16: Get Features w/SEL = 011 (SPT CAP); NSID = 00000002h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_Arbitration,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_PowerManagement,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_TemperatureThreshold,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_ErrorRecovery,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_VolatileWriteCache,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_NumberOfQueues,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_InterruptCoalescing,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_WriteAtomicityNormal,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sel	= 3,
		.status	= eSF_InvalidNamespaceOrFormat
	},
};

UINT32 HostTest_GetFeaturesParameters(void)
{
	NVME_QID	sqid = NVME_SQID_ADMIN;
	NVME_QID	cqid = NVME_CQID_ADMIN;
	UINT32	failed = 0;
	UINT32	i;

	for (i = 0; i < sizeof (testcases) / sizeof (testcases[0]); ++i) {
		HOST_MSG("Case %02d: Get Features w/SEL = %d; NSID = %08Xh; FID = %02Xh\n",
			i, testcases[i].sel, testcases[i].nsid, testcases[i].fid);

		NVME_CID cid = Host_GetFeatures(
			testcases[i].nsid,
			testcases[i].fid,
			testcases[i].sel,
			NULL,
			0);

		Host_RingDoorbell_SQT(sqid);
		NVME_STATUS status = Host_WaitForCompletion(cqid, cid)->dw3.SF;
		Host_RingDoorbell_CQH(cqid);

		if (testcases[i].status != status) {
			HOST_ERR_MSG("FAILED: expected %04xh, actual %04xh\n",
				testcases[i].status, status);
			++failed;
		}
	}

	return failed;
}
