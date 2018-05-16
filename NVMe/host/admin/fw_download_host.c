#include "nvme.h"
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
	sqe->CDW10.fwDownload.NUMD = numd;
	sqe->CDW11.fwDownload.OFST = ofst;

	Host_BuildPRP1(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

