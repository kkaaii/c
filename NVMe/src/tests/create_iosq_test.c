#include "nvme/nvme.h"
#include "nvme_host.h"
#include "nvme_device.h"

#define	SQID_VALID	2

#define	CQID_VALID	NVME_CQID_ADMIN
#define	CQID_INVALID	2

UINT32 HostTest_CreateIoSubmissionQueueParameters(void)
{
	static const struct {
		NVME_NSID	nsid;
		NVME_QID	qid;
		NVME_QID	cqid;
		UINT16		qsize;
		NVME_STATUS	status;
	} testcases[] = {
		{
			.nsid	= NVME_NSID_GLOBAL,
			.qid	= SQID_VALID,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_INVALID,
			.qid	= SQID_VALID,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_ACTIVE,
			.qid	= SQID_VALID,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidNamespaceOrFormat
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= NVME_SQID_ADMIN,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidQueueIdentifier
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= MAX_QUEUES,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_InvalidQueueIdentifier
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= SQID_VALID,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES + 1),
			.status	= eSF_InvalidQueueSize
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= SQID_VALID,
			.cqid	= CQID_INVALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_CompletionQueueInvalid
		},
		{
			.nsid	= NVME_NSID_NONE,
			.qid	= SQID_VALID,
			.cqid	= CQID_VALID,
			.qsize	= ZERO_BASED(MQES),
			.status	= eSF_SuccessfulCompletion
		},
	};

	NVME_STATUS	status;
	UINT32		i, failed = 0;
	char		*buf = Platform_MemAlign(HOST_PAGE_SIZE, HOST_PAGE_SIZE);

	for (i = 0; i < sizeof testcases / sizeof testcases[0]; ++i) {
		HOST_MSG("Case %02d: Create IOSQ w/NSID = %08Xh; QID = %04Xh; QSIZE = %04Xh; CQID = %04Xh\n",
			i + 1, testcases[i].nsid, testcases[i].qid, testcases[i].qsize, testcases[i].cqid);

		NVME_CID cid = Host_CreateIoSubmissionQueue(
			testcases[i].nsid,
			testcases[i].qid,
			testcases[i].cqid,
			buf,
			(1 + testcases[i].qsize) * sizeof (NVME_SQE));

		status = HostTest_SyncExecuteAdmin(cid);
		failed += !HostTest_CheckStatus(testcases[i].status, status);

		if (eSF_SuccessfulCompletion == status) {
			cid = Host_DeleteIoSubmissionQueue(
				testcases[i].nsid,
				testcases[i].qid);

			status = HostTest_SyncExecuteAdmin(cid);
			ASSERT(eSF_SuccessfulCompletion == status);
		}
	}

	free(buf);
	return failed;
}

