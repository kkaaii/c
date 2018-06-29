#include "nvme/nvme.h"
#include "nvme_host.h"
#include "nvme_device.h"

static struct {
	NVME_NSID	nsid;
	NVME_FID	fid;
	BOOL		sv;
	NVME_STATUS	status;
} testcases[] = {
	/* Case 17: Set Features w/SV = 0; NSID = 00000000h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_Arbitration,
		.sv	= FALSE,
		.status	= eSF_FeatureNotChangeable
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_PowerManagement,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_TemperatureThreshold,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_ErrorRecovery,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_VolatileWriteCache,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_NumberOfQueues,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_InterruptCoalescing,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= FALSE,
		.status	= eSF_FeatureNotChangeable
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 18: Set Features w/SV = 1; NSID = 00000000h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_Arbitration,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_PowerManagement,
		.sv	= TRUE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_TemperatureThreshold,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_ErrorRecovery,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_VolatileWriteCache,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_NumberOfQueues,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_InterruptCoalescing,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= TRUE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_NONE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= TRUE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 19: Set Features w/SV = 0; NSID = FFFFFFFFh */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_Arbitration,
		.sv	= FALSE,
		.status	= eSF_FeatureNotChangeable
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_PowerManagement,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_TemperatureThreshold,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_ErrorRecovery,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_VolatileWriteCache,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_NumberOfQueues,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_InterruptCoalescing,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= FALSE,
		.status	= eSF_FeatureNotChangeable
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 20: Set Features w/SV = 1; NSID = FFFFFFFFh */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_Arbitration,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_PowerManagement,
		.sv	= TRUE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_TemperatureThreshold,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_ErrorRecovery,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_VolatileWriteCache,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_NumberOfQueues,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_InterruptCoalescing,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= TRUE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_GLOBAL,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= TRUE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Case 21: Set Features w/SV = 0; NSID = 00000001h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_Arbitration,
		.sv	= FALSE,
		.status	= eSF_FeatureNotChangeable
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_PowerManagement,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_TemperatureThreshold,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_ErrorRecovery,
		.sv	= FALSE,
		.status	= eSF_SuccessfulCompletion
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_VolatileWriteCache,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_NumberOfQueues,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_InterruptCoalescing,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= FALSE,
		.status	= eSF_FeatureNotChangeable
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= FALSE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Case 22: Set Features w/SV = 1; NSID = 00000001h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_Arbitration,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_PowerManagement,
		.sv	= TRUE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_TemperatureThreshold,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_ErrorRecovery,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_VolatileWriteCache,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_NumberOfQueues,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_InterruptCoalescing,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= TRUE,
		.status	= eSF_FeatureIdentifierNotSaveable
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= TRUE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_ACTIVE,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= TRUE,
		.status	= eSF_FeatureNotNamespaceSpecific
	},
	/* Case 23: Set Features w/SV = 0; NSID = 00000002h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_Arbitration,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_PowerManagement,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_TemperatureThreshold,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_ErrorRecovery,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_VolatileWriteCache,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_NumberOfQueues,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_InterruptCoalescing,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= FALSE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Case 24: Set Features w/SV = 1; NSID = 00000002h */
	/* Sub Case 01: FID = Arbitration */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_Arbitration,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 02: FID = Power Management */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_PowerManagement,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 03: FID = Temperature Threshold */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_TemperatureThreshold,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 04: FID = Error Recovery */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_ErrorRecovery,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 05: FID = Volatile Write Cache */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_VolatileWriteCache,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 06: FID = Number of Queues */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_NumberOfQueues,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 07: FID = Interrupt Coalescing */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_InterruptCoalescing,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 08: FID = Write Atomicity */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_WriteAtomicityNormal,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 09: FID = Async Event Config */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AsynchronousEventConfiguration,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
	/* Sub Case 10: FID = Auto Pw State Trans */
	{
		.nsid	= NVME_NSID_INVALID,
		.fid	= eFID_AutonomousPowerStateTransition,
		.sv	= TRUE,
		.status	= eSF_InvalidNamespaceOrFormat
	},
};

UINT32 HostTest_SetFeaturesParameters(void)
{
	NVME_QID	sqid = NVME_SQID_ADMIN;
	NVME_QID	cqid = NVME_CQID_ADMIN;
	UINT32	failed = 0;
	UINT32	i;

	for (i = 0; i < sizeof (testcases) / sizeof (testcases[0]); ++i) {
		HOST_MSG("Case %02d: Set Features w/SV = %d; NSID = %08Xh; FID = %02Xh\n",
			i, testcases[i].sv, testcases[i].nsid, testcases[i].fid);

		NVME_CID cid = Host_GetFeatures(
			NVME_NSID_GLOBAL,
			testcases[i].fid,
			eSEL_Current,
			NULL,
			0);

		Host_RingDoorbell_SQT(sqid);
		NVME_CQE *cqe = Host_WaitForCompletion(cqid, cid);
		ASSERT(eSF_SuccessfulCompletion == cqe->dw3.SF);
		UINT32 value = cqe->dw0.all;
		Host_RingDoorbell_CQH(cqid);

		cid = Host_SetFeatures(
			testcases[i].nsid,
			testcases[i].fid,
			value,
			testcases[i].sv,
			NULL,
			0);

		Host_RingDoorbell_SQT(sqid);
		cqe = Host_WaitForCompletion(cqid, cid);
		failed += !HostTest_CheckStatus(testcases[i].status, cqe->dw3.SF);
		Host_RingDoorbell_CQH(cqid);
	}

	return failed;
}
