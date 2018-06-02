#include "nvme/nvme.h"
#include "nvme_host.h"

NVME_CID
Host_FwDownload(
	NVME_NSID	nsid,
	UINT32		ofst,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);
	UINT32		numd = (bytes >> 2) - 1;

	sqe->CDW0.OPC = NVME_OPC_ADMIN_FW_DOWNLOAD;
	sqe->NSID = nsid;
	sqe->CDW10.firmwareImageDownload.NUMD = numd;
	sqe->CDW11.firmwareImageDownload.OFST = ofst;

	Host_BuildPRPs(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

