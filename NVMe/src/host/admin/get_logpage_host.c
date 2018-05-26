#include "nvme/nvme.h"
#include "nvme_host.h"

NVME_CID
Host_GetLogPage(
	NVME_NSID	nsid,
	UINT8		lid,
	UINT64		ofst,
	void		*buf,
	UINT32		bytes)
{
	NVME_QUEUE	*asq = Host_GetSubmissionQueue(NVME_SQID_ADMIN);
	NVME_SQE	*sqe = Host_GetSubmissionQueueEntry(asq);
	UINT32		numd = (bytes >> 2) - 1;

	sqe->CDW0.OPC = NVME_OPC_ADMIN_GET_LOG_PAGE;
	sqe->NSID = nsid;
	sqe->CDW10.getLogPage.LID = lid;
	sqe->CDW10.getLogPage.NUMDL = (UINT16)numd;
	sqe->CDW11.getLogPage.NUMDU = (UINT16)(numd >> 16);
	sqe->CDW12.getLogPage.LPOL = (UINT32)ofst;
	sqe->CDW13.getLogPage.LPOU = (UINT32)(ofst >> 32);

	Host_BuildPRPs(sqe, buf, bytes);

	return Host_IssueCommand(asq);
}

