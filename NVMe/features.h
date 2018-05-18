#ifndef	_NVME_FEATURES_H
#define	_NVME_FEATURES_H

typedef enum {
	eFID_rsvd_00h,
	eFID_ARBITRATION	= 0x01,	/* Arbitration */
	eFID_POWER_MANAGEMENT	= 0x02,	/* Power Management */
	eFID_LBA_RANGE_TYPE	= 0x03,	/* LBA Range Type */
	eFID_TEMP_THRESHOLD	= 0x04,	/* Temperature Threshold */
	eFID_ERROR_RECOVERY	= 0x05,	/* Error Recovery */
	eFID_VOLATILE_WCACHE	= 0x06,	/* Volatile Write Cache */
	eFID_NUMBER_OF_QUEUES	= 0x07,	/* Number of Queues */
	eFID_INTR_COALESCING	= 0x08,	/* Interrupt Coalescing */
	eFID_INT_VEC_CONFIG	= 0x09,	/* Interrupt Vector Configuration */
	eFID_WRITE_ATOMICITY	= 0x0A,	/* Write Atomicity Normal */
	eFID_ASYNC_EVENT_CONFIG	= 0x0B,	/* Asynchronous Event Configuration */
	eFID_AUTO_PS_TRANSITION	= 0x0C,	/* Autonomous Power State Transition */
	eFID_HOST_MEMORY_BUFFER	= 0x0D,	/* Host Memory Buffer */
	eFID_rsvd_0Eh,
	eFID_KEEP_ALIVE_TIMER	= 0x0F,	/* Keep Alive Timer */
} NVME_FID;

#endif	/* _NVME_FEATURES_H */

