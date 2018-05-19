#include "nvme.h"
#include "nvme_host.h"

NVME_CID
Host_SetFeatures(
	NVME_NSID	nsid,
	NVME_FID	fid,
	UINT32		val,
	BOOL		save,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);

	sqe->CDW0.OPC = NVME_OPC_ADMIN_SET_FEATURES;
	sqe->NSID = nsid;
	sqe->CDW10.setFeatures.FID = fid;
	sqe->CDW10.setFeatures.SV = save;
	sqe->CDW11.val = val;

	Host_BuildPRP1(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

