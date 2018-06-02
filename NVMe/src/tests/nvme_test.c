#include "nvme/nvme.h"
#include "nvme_host.h"

#define	FAILED	"\033[31mFAILED\033[0m"
#define	PASSED	"\033[32mPASSED\033[0m"

BOOL HostTest_CheckStatus(NVME_STATUS expected, NVME_STATUS actual)
{
	if (expected != (actual & ~(eSF_DoNotRetry | eSF_More))) {
		HOST_MSG(FAILED": expected %04xh, actual %04xh\n", expected, actual);
		return FALSE;
	}

	HOST_MSG(PASSED": %04Xh\n", actual);
	return TRUE;
}

NVME_STATUS HostTest_SyncExecuteAdmin(NVME_CID cid)
{
	NVME_QID	cqid = NVME_CQID_ADMIN;
	NVME_QID	sqid = NVME_SQID_ADMIN;
	NVME_STATUS	status;

	Host_RingDoorbell_SQT(sqid);
	status = Host_WaitForCompletion(cqid, cid)->dw3.SF;
	Host_RingDoorbell_CQH(cqid);

	return status;
}

