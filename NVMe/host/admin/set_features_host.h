#ifndef _SET_FEATURES_HOST_H
#define	_SET_FEATURES_HOST_H

NVME_CID
Host_SetFeatures(
	NVME_NSID	nsid,
	NVME_FID	fid,
	UINT32		val,
	BOOL		sv,
	void		*buf,
	UINT32		bytes);

#endif	/* _SET_FEATURES_HOST_H */

