#ifndef	_HOST_CREATE_IOSQ_H
#define	_HOST_CREATE_IOSQ_H

NVME_CID
Host_CreateIoSubmissionQueue(
	NVME_NSID	nsid,
	NVME_QID	sqid,
	NVME_QID	cqid,
	void		*buf,
	UINT32		bytes);

#endif	/* _HOST_CREATE_IOSQ_H */

