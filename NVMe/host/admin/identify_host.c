#include "nvme.h"
#include "nvme_host.h"

NVME_CID
Host_Identify(
	NVME_NSID	nsid,
	UINT8		cns,
	NVME_CNTID	cntid,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_IDENTIFY;
	sqe->NSID = nsid;
	sqe->CDW10.identify.CNS = cns;
	sqe->CDW10.identify.CNTID = cntid;

	Host_BuildPRP1(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

