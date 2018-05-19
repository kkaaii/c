#ifndef _IDENTIFY_HOST_H
#define	_IDENTIFY_HOST_H

NVME_CID
Host_Identify(
	NVME_NSID	nsid,
	UINT8		cns,
	NVME_CNTID	cntid,
	void		*buf,
	UINT32		bytes);

#endif	/* _IDENTIFY_HOST_H */

