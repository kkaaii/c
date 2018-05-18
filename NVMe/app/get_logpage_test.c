#include "nvme.h"
#include "nvme_host.h"

#define	BYTES_16K	(16 * 1024)
#define	BYTES_4K	(4 * 1024)
#define	BYTES_512	512

UINT32 HostTest_GetLogPageParameters(void)
{
	static struct {
		NVME_NSID	nsid;	/* namespace id */
		UINT32		bytes;
		UINT8		lid;	/* Log Page id */
		NVME_STATUS	status;	/* expected status */
	} testcases[] = {
		/* Case 01: Get Log w/Length = Full Length; NSID = 00000000h */
		/* SubCase 01: LID = Error Information */
		{
			.nsid	= 0,
			.bytes	= BYTES_16K,
			.lid	= eLID_ERROR_INFORMATION,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 02: LID = SMART/Health Info */
		{
			.nsid	= 0,
			.bytes	= BYTES_512,
			.lid	= eLID_SMART_HEALTH_INFO,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 03: LID = Firmware Slot Info */
		{
			.nsid	= 0,
			.bytes	= BYTES_512,
			.lid	= eLID_FIRMWARE_SLOT_INFO,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 04: LID = Command Effects Log */
		{
			.nsid	= 0,
			.bytes	= BYTES_4K,
			.lid	= eLID_COMMAND_EFFECTS_LOG,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* Case 02: Get Log w/Length = Full Length; NSID = 00000001h */
		/* SubCase 01: LID = Error Information */
		{
			.nsid	= 1,
			.bytes	= BYTES_16K,
			.lid	= eLID_ERROR_INFORMATION,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 02: LID = SMART/Health Info */
		{
			.nsid	= 1,
			.bytes	= BYTES_512,
			.lid	= eLID_SMART_HEALTH_INFO,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 03: LID = Firmware Slot Info */
		{
			.nsid	= 1,
			.bytes	= BYTES_512,
			.lid	= eLID_FIRMWARE_SLOT_INFO,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 04: LID = Command Effects Log */
		{
			.nsid	= 1,
			.bytes	= BYTES_4K,
			.lid	= eLID_COMMAND_EFFECTS_LOG,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* Case 03: Get Log w/Length = Full Length; NSID = 00000002h */
		/* SubCase 01: LID = Error Information */
		{
			.nsid	= 2,
			.bytes	= BYTES_16K,
			.lid	= eLID_ERROR_INFORMATION,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 02: LID = SMART/Health Info */
		{
			.nsid	= 2,
			.bytes	= BYTES_512,
			.lid	= eLID_SMART_HEALTH_INFO,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 03: LID = Firmware Slot Info */
		{
			.nsid	= 2,
			.bytes	= BYTES_512,
			.lid	= eLID_FIRMWARE_SLOT_INFO,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 04: LID = Command Effects Log */
		{
			.nsid	= 2,
			.bytes	= BYTES_4K,
			.lid	= eLID_COMMAND_EFFECTS_LOG,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* Case 04: Get Log w/Length = Full Length; NSID = FFFFFFFFh */
		/* SubCase 01: LID = Error Information */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= BYTES_16K,
			.lid	= eLID_ERROR_INFORMATION,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 02: LID = SMART/Health Info */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= BYTES_512,
			.lid	= eLID_SMART_HEALTH_INFO,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 03: LID = Firmware Slot Info */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= BYTES_512,
			.lid	= eLID_FIRMWARE_SLOT_INFO,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 04: LID = Command Effects Log */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= BYTES_4K,
			.lid	= eLID_COMMAND_EFFECTS_LOG,
			.status	= eSF_SuccessfulCompletion
		},
		/* Case 05: Get Log w/Length = Zero Length; NSID = FFFFFFFFh */
		/* SubCase 01: LID = Error Information */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= 0,
			.lid	= eLID_ERROR_INFORMATION,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 02: LID = SMART/Health Info */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= 0,
			.lid	= eLID_SMART_HEALTH_INFO,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 03: LID = Firmware Slot Info */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= 0,
			.lid	= eLID_FIRMWARE_SLOT_INFO,
			.status	= eSF_SuccessfulCompletion
		},
		/* SubCase 04: LID = Command Effects Log */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.bytes	= 0,
			.lid	= eLID_COMMAND_EFFECTS_LOG,
			.status	= eSF_SuccessfulCompletion
		},
	};

	UINT32	i;
	UINT32	failed = 0;
	UINT8	buf[4096];

	for (i = 0; i < sizeof (testcases) / sizeof (testcases[0]); ++i) {
		HOST_PRN_MSG("Case %02d: Get Log w/Length = %04Xh; NSID = %08Xh; LID = %02Xh\n",
			i, testcases[i].bytes, testcases[i].nsid, testcases[i].lid);
		NVME_CID cid = Host_GetLogPage(
			testcases[i].nsid,
			testcases[i].lid,
			0,
			buf,
			testcases[i].bytes);

		Host_RingDoorbell_SQT(NVME_SQID_ADMIN);
		NVME_STATUS status = Host_WaitForCompletion(NVME_CQID_ADMIN, cid);
		if (testcases[i].status != status) {
			HOST_PRN_MSG("FAILED: expected %04xh, actual %04xh\n",
				testcases[i].status, status);
			++failed;
		}
		Host_RingDoorbell_CQH(NVME_CQID_ADMIN);
	}

	return failed;
}

