#include "nvme/nvme.h"
#include "nvme_host.h"

UINT32 HostTest_FwDownloadParameters(void)
{
	static struct {
		NVME_NSID	nsid;	/* namespace id */
                UINT32          numd;   /* number of dwords */
                UINT32          ofst;   /* offset */
		NVME_STATUS	status;	/* expected status */
	} testcases[] = {
		/* Case 01: FIRMWARE DOWNLOAD w/NSID = 00000000h */
		{
			.nsid	= 0,
			.status	= NVME_STATUS_SUCCESSFUL_COMPLETION
		},
		/* Case 02: FIRMWARE DOWNLOAD w/NSID = 00000001h */
		{
			.nsid	= 1,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		},
		/* Case 03: FIRMWARE DOWNLOAD w/NSID = 00000002h */
		{
			.nsid	= 2,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		},
		/* Case 04: FIRMWARE DOWNLOAD w/NSID = FFFFFFFFh */
		{
			.nsid	= NVME_NSID_GLOBAL,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		}
	};

	UINT32	i;
	UINT32	failed = 0;
	UINT8	buf[4096];

	for (i = 0; i < sizeof (testcases) / sizeof (testcases[0]); ++i) {
		HOST_MSG("Case %02d: FIRMWARE DOWNLOAD w/NSID = %08Xh\n", i, testcases[i].nsid);
		NVME_CID cid = Host_FwDownload(
			testcases[i].nsid,
                        0,
			buf,
			sizeof buf);

		Host_RingDoorbell_SQT(NVME_SQID_ADMIN);
		NVME_STATUS status = Host_WaitForCompletion(NVME_CQID_ADMIN, cid)->dw3.SF;
		if (testcases[i].status != status) {
			HOST_MSG("FAILED: expected %04xh, actual %04xh\n",
				testcases[i].status, status);
			++failed;
		}
		Host_RingDoorbell_CQH(NVME_CQID_ADMIN);
	}

	return failed;
}

