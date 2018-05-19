#ifndef _FW_DOWNLOAD_HOST_H
#define	_FW_DOWNLOAD_HOST_H

NVME_CID
Host_FwDownload(
	NVME_NSID	nsid,
	UINT32		ofst,
	void		*buf,
	UINT32		bytes);

#endif	/* _FW_DOWNLOAD_HOST_H */

