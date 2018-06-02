#include "nvme/nvme.h"
#include "nvme_host.h"
#include "nvme_device.h"

#define	CQID_VALID	2
#define	CQID_INVALID	3

#define	SQID_TEST	2

UINT32 HostTest_DeleteIoCompletionQueueParameters(void)
{
	static const struct {
		NVME_NSID	nsid;
		NVME_QID	qid;
		NVME_STATUS	status;
	} testcases[] = {
		{
			.nsid	= NVME_NSID_GLOBAL,
			.qid	= CQID_VALID,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_INVALID,
			.qid	= CQID_VALID,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_ACTIVE,
			.qid	= CQID_VALID,
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= NVME_CQID_ADMIN,
			.status	= eSF_InvalidQueueIdentifier
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= CQID_INVALID,
			.status	= eSF_InvalidQueueDeletion
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= CQID_VALID,
			.status	= eSF_SuccessfulCompletion
		},
	};

	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_CID	cid;
	UINT32		i, failed = 0;
	char		*buf = Platform_MemAlign(HOST_PAGE_SIZE, HOST_PAGE_SIZE * 3);

	cid = Host_CreateIoCompletionQueue(
		NVME_NSID_NONE,
		CQID_INVALID,
		buf + HOST_PAGE_SIZE,
		HOST_PAGE_SIZE / 4);

	status = HostTest_SyncExecuteAdmin(cid);
	ASSERT(eSF_SuccessfulCompletion == status);

	cid = Host_CreateIoSubmissionQueue(
		NVME_NSID_NONE,
		SQID_TEST,
		CQID_INVALID,
		buf + HOST_PAGE_SIZE * 2,
		HOST_PAGE_SIZE);

	status = HostTest_SyncExecuteAdmin(cid);
	ASSERT(eSF_SuccessfulCompletion == status);

	for (i = 0; i < sizeof testcases / sizeof testcases[0]; ++i) {
		HOST_MSG("Case %02d: Delete IOCQ w/NSID = %08Xh; QID = %04Xh\n",
			i + 1, testcases[i].nsid, testcases[i].qid);

		if (eSF_SuccessfulCompletion == status) {
			cid = Host_CreateIoCompletionQueue(
				NVME_NSID_NONE,
				CQID_VALID,
				buf,
				HOST_PAGE_SIZE / 4);

			status = HostTest_SyncExecuteAdmin(cid);
			ASSERT(eSF_SuccessfulCompletion == status);
		}

		cid = Host_DeleteIoCompletionQueue(
			testcases[i].nsid,
			testcases[i].qid);

		status = HostTest_SyncExecuteAdmin(cid);
		failed += !HostTest_CheckStatus(testcases[i].status, status);
	}

	cid = Host_DeleteIoSubmissionQueue(NVME_NSID_NONE, SQID_TEST);
	status = HostTest_SyncExecuteAdmin(cid);
	ASSERT(eSF_SuccessfulCompletion == status);

	cid = Host_DeleteIoCompletionQueue(NVME_NSID_NONE, CQID_INVALID);
	status = HostTest_SyncExecuteAdmin(cid);
	ASSERT(eSF_SuccessfulCompletion == status);

	free(buf);
	return failed;
}

