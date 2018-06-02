#include "nvme/nvme.h"
#include "nvme_host.h"
#include "nvme_device.h"

#define	CQID_TEST	2

UINT32 HostTest_CreateIoCompletionQueueParameters(void)
{
	static const struct {
		NVME_NSID	nsid;
		NVME_QID	qid;
		UINT16		qsize;
		NVME_STATUS	status;
	} testcases[] = {
		{
			.nsid	= NVME_NSID_GLOBAL,
			.qid	= CQID_TEST,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_INVALID,
			.qid	= CQID_TEST,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_ACTIVE,
			.qid	= CQID_TEST,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= NVME_CQID_ADMIN,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidQueueIdentifier
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= CQID_TEST,
			.qsize	= ZERO_BASED(MQES + 1),
			.status	= eSF_InvalidQueueSize
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= CQID_TEST,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_SuccessfulCompletion
		},
	};

	NVME_STATUS	status;
	UINT32		i, failed = 0;
	char		*buf = Platform_MemAlign(HOST_PAGE_SIZE, HOST_PAGE_SIZE);

	for (i = 0; i < sizeof testcases / sizeof testcases[0]; ++i) {
		HOST_MSG("Case %02d: Create IOCQ w/NSID = %08Xh; QID = %04Xh; QSIZE = %04Xh\n",
			i + 1, testcases[i].nsid, testcases[i].qid, testcases[i].qsize);

		NVME_CID cid = Host_CreateIoCompletionQueue(
			testcases[i].nsid,
			testcases[i].qid,
			buf,
			(1 + testcases[i].qsize) * sizeof (NVME_CQE));

		status = HostTest_SyncExecuteAdmin(cid);
		failed += !HostTest_CheckStatus(testcases[i].status, status);

		if (eSF_SuccessfulCompletion == status) {
			cid = Host_DeleteIoCompletionQueue(
				testcases[i].nsid,
				testcases[i].qid);

			status = HostTest_SyncExecuteAdmin(cid);
			ASSERT(eSF_SuccessfulCompletion == status);
		}
	}

	free(buf);
	return failed;
}

