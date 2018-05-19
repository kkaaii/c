#ifndef	_HOST_FEATURES_H
#define	_HOST_FEATURES_H

NVME_CID
Host_GetFeatures(
	NVME_NSID	nsid,
	NVME_FID	fid,
	UINT8		sel,
	void		*buf,
	UINT32		bytes);

NVME_CID
Host_SetFeatures(
	NVME_NSID	nsid,
	NVME_FID	fid,
	UINT32		val,
	BOOL		save,
	void		*buf,
	UINT32		bytes);

#endif	/* _HOST_FEATURES_H */

