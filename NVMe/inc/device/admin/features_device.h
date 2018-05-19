#ifndef _FEATURES_DEVICE_H
#define _FEATURES_DEVICE_H

BOOL Device_GetFeatures(NVME_QID sqid, NVME_QID cqid);
BOOL Device_SetFeatures(NVME_QID sqid, NVME_QID cqid);

#endif  /* FEATURES_DEVICE_H */
