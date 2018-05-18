#ifndef _GET_FEATURES_HOST_H
#define	_GET_FEATURES_HOST_H

NVME_CID
Host_GetFeatures(
	NVME_NSID	nsid,
	NVME_FID	fid,
	UINT8		sel,
	UINT32		dw11,
	void		*buf,
	UINT32		bytes);

#endif	/* _GET_FEATURES_HOST_H */

