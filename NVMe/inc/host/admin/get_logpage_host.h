#ifndef _GET_LOGPAGE_HOST_H
#define	_GEt_LOGPAGE_HOST_H

NVME_CID
Host_GetLogPage(
	NVME_NSID	nsid,
	UINT8		lid,
	UINT64		ofst,
	void		*buf,
	UINT32		bytes);

#endif	/* _GET_LOGPAGE_HOST_H */

