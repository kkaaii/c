#include "nvme.h"
#include "nvme_host.h"

NVME_CID
Host_GetFeatures(
	NVME_NSID	nsid,
	NVME_FID	fid,
	UINT8		sel,
	UINT32		dw11,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_GET_FEATURES;
	sqe->NSID = nsid;
	sqe->CDW10.getFeatures.FID = fid;
	sqe->CDW10.getFeatures.SEL = sel;
	sqe->CDW11.val = dw11;

	Host_BuildPRP1(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

