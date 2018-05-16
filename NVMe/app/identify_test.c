#include "nvme.h"
#include "nvme_host.h"

#define	NSID_ACTIVE	1
#define	NSID_INVALID	2

UINT32 HostTest_IdentifyParameters(void)
{
	static struct {
		NVME_NSID	nsid;	/* namespace id */
		NVME_CNTID	cntid;	/* controller id */
		UINT8		cns;
		NVME_STATUS	status;	/* expected status */
	} testcases[] = {
		/* Case 01: Identify CNS = 1; NSID = 0; CNTID = 0 */
		{
			.nsid	= 0,
			.cntid	= 0,
			.cns	= 1,
			.status	= NVME_STATUS_SUCCESSFUL_COMPLETION
		},
		/* Case 02: Identify CNS = 1; NSID != 0; CNTID = 0 */
		{
			.nsid	= 1,
			.cntid	= 0,
			.cns	= 1,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		},
		/* Case 03: Identify CNS = 1; NSID = 0; CNTID != 0 */
		{
			.nsid	= 0,
			.cntid	= 1,
			.cns	= 1,
			.status	= NVME_STATUS_INVALID_FIELD_IN_COMMAND
		},
		/* Case 04: Identify CNS = 2; NSID = 1; CNTID = 0 */
		{
			.nsid	= 1,
			.cntid	= 0,
			.cns	= 2,
			.status	= NVME_STATUS_SUCCESSFUL_COMPLETION
		},
		/* Case 05: Identify CNS = 2; NSID = Active; CNTID = 0 */
		{
			.nsid	= NSID_ACTIVE,
			.cntid	= 0,
			.cns	= 2,
			.status	= NVME_STATUS_SUCCESSFUL_COMPLETION
		},
		/* Case 06: Identify CNS = 2; NSID = FFFFFFFFh; CNTID = 0 */
		{
			.nsid	= 0xFFFFFFFF,
			.cntid	= 0,
			.cns	= 2,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		},
		/* Case 07: Identify CNS = 2; NSID = FFFFFFFFh; CNTID = 0 */
		{
			.nsid	= 0xFFFFFFFE,
			.cntid	= 0,
			.cns	= 2,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		},
		/* Case 08: Identify CNS = 2; NSID = 1; CNTID != 0 */
		{
			.nsid	= 1,
			.cntid	= 1,
			.cns	= 2,
			.status	= NVME_STATUS_INVALID_FIELD_IN_COMMAND
		},
		/* Case 09: Identify CNS = 0; NSID = Active; CNTID = 0 */
		{
			.nsid	= NSID_ACTIVE,
			.cntid	= 0,
			.cns	= 0,
			.status	= NVME_STATUS_SUCCESSFUL_COMPLETION
		},
		/* Case 10: Identify CNS = 0; NSID = Invalid; CNTID = 0 */
		{
			.nsid	= NSID_INVALID,
			.cntid	= 0,
			.cns	= 0,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		},
		/* Case 11: Identify CNS = 0; NSID = Active; CNTID != 0 */
		{
			.nsid	= NSID_ACTIVE,
			.cntid	= 1,
			.cns	= 0,
			.status	= NVME_STATUS_INVALID_FIELD_IN_COMMAND
		},
		/* Case 12: Identify CNS = 0; NSID = FFFFFFFFh; CNTID = 0 */
		{
			.nsid	= 0xFFFFFFFF,
			.cntid	= 0,
			.cns	= 0,
			.status	= NVME_STATUS_INVALID_NAMESPACE_OR_FORMAT
		}
	};

	UINT32	i;
	UINT32	failed = 0;
	UINT8	buf[4096];

	for (i = 0; i < sizeof (testcases) / sizeof (testcases[0]); ++i) {
		HOST_PRN_MSG("Case %02d: Identify NSID = %08Xh; CNS = %d; CNTID = %d\n",
			i, testcases[i].nsid, testcases[i].cns, testcases[i].cntid);
		NVME_CID cid = Host_Identify(
			testcases[i].nsid,
			testcases[i].cns,
			testcases[i].cntid,
			buf,
			sizeof buf);

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

