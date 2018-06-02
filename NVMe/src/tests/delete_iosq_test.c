#include "nvme/nvme.h"
#include "nvme_host.h"
#include "nvme_device.h"

#define	SQID_VALID	2

UINT32 HostTest_DeleteIoSubmissionQueueParameters(void)
{
	static const struct {
		NVME_NSID	nsid;
		NVME_QID	qid;
		NVME_STATUS	status;
	} testcases[] = {
		{
			.nsid	= NVME_NSID_GLOBAL,
			.qid	= SQID_VALID,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_INVALID,
			.qid	= SQID_VALID,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_ACTIVE,
			.qid	= SQID_VALID,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= NVME_SQID_ADMIN,
			.status	= eSF_InvalidQueueIdentifier
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= SQID_VALID,
			.status	= eSF_SuccessfulCompletion
		},
	};

	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_CID	cid;
	UINT32		i, failed = 0;
	char		*buf = Platform_MemAlign(HOST_PAGE_SIZE, HOST_PAGE_SIZE);

	for (i = 0; i < sizeof testcases / sizeof testcases[0]; ++i) {
		HOST_MSG("Case %02d: Delete IOSQ w/NSID = %08Xh; QID = %04Xh\n",
			i + 1, testcases[i].nsid, testcases[i].qid);

		if (eSF_SuccessfulCompletion == status) {
			cid = Host_CreateIoSubmissionQueue(
				NVME_NSID_NONE,
				SQID_VALID,
				NVME_CQID_ADMIN,
				buf,
				HOST_PAGE_SIZE);

			status = HostTest_SyncExecuteAdmin(cid);
			ASSERT(eSF_SuccessfulCompletion == status);
		}

		cid = Host_DeleteIoSubmissionQueue(
			testcases[i].nsid,
			testcases[i].qid);

		status = HostTest_SyncExecuteAdmin(cid);
		failed += !HostTest_CheckStatus(testcases[i].status, status);
	}

	free(buf);
	return failed;
}

