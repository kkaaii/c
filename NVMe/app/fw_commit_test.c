#include "nvme.h"
#include "nvme_host.h"

UINT32 HostTest_FwCommitParameters(void)
{
	static struct {
		NVME_NSID	nsid;	/* namespace id */
		UINT8		ca;	/* commit action */
		UINT8		fs;	/* firmware slot */
		NVME_STATUS	status;	/* expected status */
	} testcases[] = {
		/* Case 05: FIRMWARE COMMIT w/NSID = 00000000h */
		/* SubCase 01: CA = 000b; FS = 0 */
		{
			.nsid	= 0,
			.ca	= 0,
			.fs	= 0,
			.status	= eSF_InvalidFirmwareImage
		},
		/* SubCase 02: CA = 000b; FS = 1 */
		{
			.nsid	= 0,
			.ca	= 0,
			.fs	= 1,
			.status	= eSF_InvalidFirmwareImage
		},
		/* SubCase 03: CA = 000b; FS = 2 */
		{
			.nsid	= 0,
			.ca	= 0,
			.fs	= 2,
			.status	= eSF_InvalidFirmwareSlot
		},
		/* SubCase 04: CA = 001b; FS = 0 */
		{
			.nsid	= 0,
			.ca	= 1,
			.fs	= 0,
			.status	= eSF_InvalidFirmwareImage
		},
		/* SubCase 05: CA = 001b; FS = 1 */
		{
			.nsid	= 0,
			.ca	= 1,
			.fs	= 1,
			.status	= eSF_InvalidFirmwareImage
		},
		/* SubCase 06: CA = 001b; FS = 2 */
		{
			.nsid	= 0,
			.ca	= 1,
			.fs	= 2,
			.status	= eSF_InvalidFirmwareSlot
		},
		/* SubCase 07: CA = 010b; FS = 0 */
		{
			.nsid	= 0,
			.ca	= 2,
			.fs	= 0,
			.status	= eSF_InvalidFirmwareImage
		},
		/* SubCase 08: CA = 010b; FS = 1 */
		{
			.nsid	= 0,
			.ca	= 2,
			.fs	= 1,
			.status	= eSF_InvalidFirmwareImage
		},
		/* SubCase 09: CA = 010b; FS = 2 */
		{
			.nsid	= 0,
			.ca	= 2,
			.fs	= 2,
			.status	= eSF_InvalidFirmwareSlot
		},
		/* SubCase 10: CA = 011b; FS = 0 */
		{
			.nsid	= 0,
			.ca	= 3,
			.fs	= 0,
			.status	= eSF_InvalidFieldInCommand
		},
		/* SubCase 11: CA = 011b; FS = 1 */
		{
			.nsid	= 0,
			.ca	= 3,
			.fs	= 1,
			.status	= eSF_InvalidFieldInCommand
		},
		/* SubCase 12: CA = 011b; FS = 2 */
		{
			.nsid	= 0,
			.ca	= 3,
			.fs	= 2,
			.status	= eSF_InvalidFieldInCommand
		},
		/* Case 06: FIRMWARE COMMIT w/NSID = 00000001h */
		/* SubCase 01: CA = 000b; FS = 0 */
		{
			.nsid	= 1,
			.ca	= 0,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 02: CA = 000b; FS = 1 */
		{
			.nsid	= 1,
			.ca	= 0,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 03: CA = 001b; FS = 0 */
		{
			.nsid	= 1,
			.ca	= 1,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 04: CA = 001b; FS = 1 */
		{
			.nsid	= 1,
			.ca	= 1,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 05: CA = 010b; FS = 0 */
		{
			.nsid	= 1,
			.ca	= 2,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 06: CA = 010b; FS = 1 */
		{
			.nsid	= 1,
			.ca	= 2,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 07: CA = 011b; FS = 0 */
		{
			.nsid	= 1,
			.ca	= 3,
			.fs	= 0,
			.status	= eSF_InvalidFieldInCommand
		},
		/* SubCase 08: CA = 011b; FS = 1 */
		{
			.nsid	= 1,
			.ca	= 3,
			.fs	= 1,
			.status	= eSF_InvalidFieldInCommand
		},
		/* Case 07: FIRMWARE COMMIT w/NSID = 00000002h */
		/* SubCase 01: CA = 000b; FS = 0 */
		{
			.nsid	= 2,
			.ca	= 0,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 02: CA = 000b; FS = 1 */
		{
			.nsid	= 2,
			.ca	= 0,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 03: CA = 001b; FS = 0 */
		{
			.nsid	= 2,
			.ca	= 1,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 04: CA = 001b; FS = 1 */
		{
			.nsid	= 2,
			.ca	= 1,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 05: CA = 010b; FS = 0 */
		{
			.nsid	= 2,
			.ca	= 2,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 06: CA = 010b; FS = 1 */
		{
			.nsid	= 2,
			.ca	= 2,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 07: CA = 011b; FS = 0 */
		{
			.nsid	= 2,
			.ca	= 3,
			.fs	= 0,
			.status	= eSF_InvalidFieldInCommand
		},
		/* SubCase 08: CA = 011b; FS = 1 */
		{
			.nsid	= 2,
			.ca	= 3,
			.fs	= 1,
			.status	= eSF_InvalidFieldInCommand
		},
		/* Case 08: FIRMWARE COMMIT w/NSID = FFFFFFFFh */
		/* SubCase 01: CA = 000b; FS = 0 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 0,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 02: CA = 000b; FS = 1 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 0,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 03: CA = 001b; FS = 0 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 1,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 04: CA = 001b; FS = 1 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 1,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 05: CA = 010b; FS = 0 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 2,
			.fs	= 0,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 06: CA = 010b; FS = 1 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 2,
			.fs	= 1,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		/* SubCase 07: CA = 011b; FS = 0 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 3,
			.fs	= 0,
			.status	= eSF_InvalidFieldInCommand
		},
		/* SubCase 08: CA = 011b; FS = 1 */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.ca	= 3,
			.fs	= 1,
			.status	= eSF_InvalidFieldInCommand
		}
	};

	UINT32	i;
	UINT32	failed = 0;
	UINT8	buf[4096];

	for (i = 0; i < sizeof (testcases) / sizeof (testcases[0]); ++i) {
		HOST_PRN_MSG("Case %02d: FIRMWARE COMMIT w/NSID = %08Xh; CA = %d; FS = %d\n",
			i, testcases[i].nsid, testcases[i].ca, testcases[i].fs);
		NVME_CID cid = Host_FwCommit(
			testcases[i].nsid,
			testcases[i].ca,
			testcases[i].fs);

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

