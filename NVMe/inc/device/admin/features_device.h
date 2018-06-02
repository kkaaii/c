#ifndef _FEATURES_DEVICE_H
#define _FEATURES_DEVICE_H

#define	FEAT_ARB_AB		0
#define	FEAT_ARB_LPW		0
#define	FEAT_ARB_MPW		0
#define	FEAT_ARB_HPW		0

#define	FEAT_ARB_DEFAULT	(\
	(FEAT_ARB_AB	<< 0)	|\
	(FEAT_ARB_LPW	<< 8)	|\
	(FEAT_ARB_MPW	<< 16)	|\
	(FEAT_ARB_HPW	<< 24)	)

#define	FEAT_NQ_NSQR		ZERO_BASED(MAX_QUEUES - 1)
#define	FEAT_NQ_NCQR		ZERO_BASED(MAX_QUEUES - 1)

#define	FEAT_NQ_DEFAULT		(\
	(FEAT_NQ_NSQR	<< 0)	|\
	(FEAT_NQ_NCQR	<< 16)	)

UINT32	Device_GetFeatureDword(NVME_FID fid, NVME_SEL sel);

BOOL Device_GetFeatures(NVME_QID sqid, NVME_QID cqid);
BOOL Device_SetFeatures(NVME_QID sqid, NVME_QID cqid);

#endif  /* FEATURES_DEVICE_H */
