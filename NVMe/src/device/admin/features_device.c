#include "nvme/nvme.h"
#include "nvme_device.h"

typedef	struct {
	NVME_FID	fid;
	UINT32		vCurrent;
	UINT32		vDefault;
	UINT32		vSaved;
	UINT32		vCapabilities;
} Feature;

CC_STATIC	Feature	supported[] = {
	{
		.fid		= /* 01h */eFID_Arbitration,
		.vCurrent	= FEAT_ARB_DEFAULT,
		.vCapabilities	= 0
	}, {
		.fid		= /* 02h */eFID_PowerManagement,
		.vCapabilities	= eCAP_Changeable | eCAP_Saveable
#if 0
	}, {
		.fid		= /* 03h */eFID_LbaRangeType,
		.vCapabilities	= 0
#endif
	}, {
		.fid		= /* 04h */eFID_TemperatureThreshold,
		.vCapabilities	= eCAP_Changeable
	}, {
		.fid		= /* 05h */eFID_ErrorRecovery,
		.vCapabilities	= eCAP_Changeable | eCAP_NamespaceSpecific
	}, {
		.fid		= /* 06h */eFID_VolatileWriteCache,
		.vCapabilities	= eCAP_Changeable
	}, {
		.fid		= /* 07h */eFID_NumberOfQueues,
		.vCurrent	= FEAT_NQ_DEFAULT,
		.vCapabilities	= eCAP_Changeable
	}, {
		.fid		= /* 08h */eFID_InterruptCoalescing,
		.vCapabilities	= eCAP_Changeable
	}, {
		.fid		= /* 09h */eFID_InterruptVectorConfiguration,
		.vCapabilities	= eCAP_Changeable
	}, {
		.fid		= /* 0Ah */eFID_WriteAtomicityNormal,
		.vCapabilities	= 0
	}, {
		.fid		= /* 0Bh */eFID_AsynchronousEventConfiguration,
		.vCapabilities	= eCAP_Changeable | eCAP_Saveable
	}, {
		.fid		= /* 0Ch */eFID_AutonomousPowerStateTransition,
		.vCapabilities	= eCAP_Changeable | eCAP_Saveable
	}
};

CC_STATIC_ALWAYS_INLINE
BOOL Feature_IsNamespaceSpecific(Feature *feature)
{
	return (0 != (feature->vCapabilities & eCAP_NamespaceSpecific));
}

CC_STATIC_ALWAYS_INLINE
BOOL Feature_IsSaveable(Feature *feature)
{
	return (0 != (feature->vCapabilities & eCAP_Saveable));
}

CC_STATIC
Feature *Device_GetFeature(NVME_FID fid)
{
	UINT32	i;

	for (i = 0; i < sizeof (supported) / sizeof (supported[0]); ++i) {
		if (fid == supported[i].fid) {
			return &supported[i];
		}
	}

	return NULL;
}

UINT32	Device_GetFeatureDword(NVME_FID fid, NVME_SEL sel)
{
	Feature	*feature = Device_GetFeature(fid);
	if (NULL == feature)
		return 0;

	switch (sel) {
	case eSEL_Current:
		return feature->vCurrent;

	case eSEL_Default:
		return feature->vDefault;

	case eSEL_Saved:
		return Feature_IsSaveable(feature) ? feature->vSaved : feature->vDefault;

	default:
		break;
	}

	return 0;
}

BOOL Device_GetFeatures(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);
	NVME_FID	fid = sqe->CDW10.getFeatures.FID;
	NVME_SEL	sel = sqe->CDW10.getFeatures.SEL;

	DEV_DBG_MSG("Get Features: FID = %02Xh; SEL = %01Xh\n", fid, sel);

	do {
		Feature	*feature = Device_GetFeature(fid);
		if (NULL == feature) {
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			break;
		}

		if (NVME_NSID_INVALID <= sqe->NSID && NVME_NSID_GLOBAL != sqe->NSID) {
			status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
			break;
		}

		UINT32 value = Device_GetFeatureDword(fid, sel);
		Device_SetCommandSpecific(cq, value);
	} while (FALSE);

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

BOOL Device_SetFeatures(NVME_QID sqid, NVME_QID cqid)
{
	NVME_STATUS	status = eSF_SuccessfulCompletion;
	NVME_QUEUE	*sq = Device_GetSubmissionQueue(sqid);
	NVME_QUEUE	*cq = Device_GetCompletionQueue(cqid);
	ASSERT(NULL != sq && NULL != cq);

	NVME_SQE	*sqe = Device_GetSubmissionQueueEntry(sq);
	NVME_FID	fid = sqe->CDW10.setFeatures.FID;

	DEV_DBG_MSG("Set Features: FID = %02Xh; value = %08Xh\n", fid, sqe->CDW11.val);

	do {
		Feature	*feature = Device_GetFeature(fid);
		if (NULL == feature) {
			status = eSF_DoNotRetry | eSF_InvalidFieldInCommand;
			break;
		}

		if (sqe->CDW10.setFeatures.SV && !Feature_IsSaveable(feature)) {
			status = eSF_DoNotRetry | eSF_FeatureIdentifierNotSaveable;
			break;
		}

		if (NVME_NSID_NONE != sqe->NSID && NVME_NSID_GLOBAL != sqe->NSID) {
			if (NVME_NSID_INVALID <= sqe->NSID) {
				status = eSF_DoNotRetry | eSF_InvalidNamespaceOrFormat;
				break;
			}

			if (!Feature_IsNamespaceSpecific(feature)) {
				status = eSF_DoNotRetry | eSF_FeatureNotNamespaceSpecific;
				break;
			}
		}

		feature->vCurrent = sqe->CDW11.val;
	} while (FALSE);

	Device_SetNvmeStatus(cq, status);
	Device_ChangeState(eDeviceState_ReturnStatus);
	return TRUE;
}

