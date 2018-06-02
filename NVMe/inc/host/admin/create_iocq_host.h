#ifndef	_HOST_CREATE_IOCQ_H
#define	_HOST_CREATE_IOCQ_H

NVME_CID
Host_CreateIoCompletionQueue(
	NVME_NSID	nsid,
	NVME_QID	cqid,
	void		*buf,
	UINT32		bytes);

#endif	/* _HOST_CREATE_IOCQ_H */

