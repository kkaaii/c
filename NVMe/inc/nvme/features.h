#ifndef _NVME_FEATURES_H
#define	_NVME_FEATURES_H

/*
** Figure 70: Get Features - Command Dword 10
*/
typedef	enum {
	/* 000b */eSEL_Current,
	/* 001b */eSEL_Default,
	/* 010b */eSEL_Saved,
	/* 011b */eSEL_SupportedCapabilities,
	/* 100b */eSEL_reserved4,
	/* 101b */eSEL_reserved5,
	/* 110b */eSEL_reserved6,
	/* 111b */eSEL_reserved7,
} NVME_SEL;

enum {
	eCAP_Saveable		= BIT(0),
	eCAP_NamespaceSpecific	= BIT(1),
	eCAP_Changeable		= BIT(2)
};

typedef enum {
/*
** Figure 105: Set Features - Feature Identifiers
*/
	/* 00h */eFID_Reserved_00h	= 0x00,
	/* 01h */eFID_Arbitration,
	/* 02h */eFID_PowerManagement,
	/* 03h */eFID_LbaRangeType,
	/* 04h */eFID_TemperatureThreshold,
	/* 05h */eFID_ErrorRecovery,
	/* 06h */eFID_VolatileWriteCache,
	/* 07h */eFID_NumberOfQueues,
	/* 08h */eFID_InterruptCoalescing,
	/* 09h */eFID_InterruptVectorConfiguration,
	/* 0Ah */eFID_WriteAtomicityNormal,
	/* 0Bh */eFID_AsynchronousEventConfiguration,
	/* 0Ch */eFID_AutonomousPowerStateTransition,
	/* 0Dh */eFID_HostMemoryBuffer,
	/* 0Eh */eFID_Timestamp,			/* 1.3 - */
	/* 0Fh */eFID_KeepAliveTimer,
	/* 10h */eFID_HostControlledThermalManagement,	/* 1.3 - */
	/* 11h */eFID_NonOperationalPowerStateConfig,	/* 1.3 - */

/*
** Figure 106: Set Features, NVM Command Set Specific - Feature Identifiers
*/
	/* 80h */eFID_SoftwareProgressMarker	= 0x80,
	/* 81h */eFID_HostIdentifier,
	/* 82h */eFID_ReservationNotificationMask,
	/* 83h */eFID_ReservationPersistance,
} NVME_FID;

/*
** Figure 107: Arbitration & Command Processing - Command Dword 11
*/
typedef	struct {
	UINT32	AB	: 3;	/* Arbitration Burst */
	UINT32	rsvd3	: 5;
	UINT32	LPW	: 8;	/* Low Priority Weight */
	UINT32	MPW	: 8;	/* Medium Priority Weight */
	UINT32	HPW	: 8;	/* High Priority Weight */
} FEAT_ARB;

/*
** Figure 108: Power Management - Command Dword 11
*/
typedef	struct {
	UINT32	PS	: 5;	/* Power State */
	UINT32	WH	: 3;	/* Workload Hint */
	UINT32	rsvd8	: 24;
} FEAT_PM;

typedef enum {
	eTTS_CompositeTemperature	= 0,
	eTTS_TemperatureSensor1,
	eTTS_TemperatureSensor2,
	eTTS_TemperatureSensor3,
	eTTS_TemperatureSensor4,
	eTTS_TemperatureSensor5,
	eTTS_TemperatureSensor6,
	eTTS_TemperatureSensor7,
	eTTS_TemperatureSensor8,

	eTTS_TemperatureAllSensors	= 15
} ETemperatureThresholdSelect;

typedef enum {
	eTTT_OverTemperatureThreshold	= 0,
	eTTT_UnderTemperatureThreshold
} ETemperatureThresholdType;

/*
** Figure 110: Temperature Threshold - Command Dword 11
*/
typedef struct {
	UINT32	TMPTH	: 16;	/* Temperature Threshold */
	UINT32	TMPSEL	: 4;	/* Threshold Temperature Select */
	UINT32	THSEL	: 2;	/* Threshold Type Select */
	UINT32	rsvd22	: 10;
} FEAT_TT;

/*
** Figure 112: Error Recovery - Command Dword 11
*/
typedef	struct {
	UINT32	TLER	: 16;	/* Time Limited Error Recovery */
	UINT32	DULBE	: 1;	/* Deallocated or Unwritten Logical Block Error Enable */
	UINT32	rsvd17	: 15;
} FEAT_ER;

/*
** Figure 113: Volatile Write Cache - Command Dword 11
*/
typedef	struct {
	UINT32	WCE	: 1;	/* Volatile Write Cache Enable */
	UINT32	rsvd1	: 31;
} FEAT_VWC;

/*
** Figure 114: Number of Queues - Command Dword 11
*/
typedef	struct {
	UINT32	NSQR	: 16;	/* Number of I/O Submission Queues Requested */
	UINT32	NCQR	: 16;	/* Number of I/O Completion Queues Requested */
} FEAT_NQ;

/*
** Figure 116: Interrupt Coalescing - Command Dword 11
*/
typedef	struct {
	UINT32	THR	: 8;	/* Aggregation Threshold */
	UINT32	TIME	: 8;	/* Aggregation Time */
	UINT32	rsvd16	: 16;
} FEAT_IC;

/*
** Figure 117: Interrupt Vector Configuration - Command Dword 11
*/
typedef	struct {
	UINT32	IV	: 16;	/* Interrupt Vector */
	UINT32	CD	: 1;	/* Coalescing Disable */
	UINT32	rsvd17	: 15;
} FEAT_IVC;

/*
** Figure 118: Write Atomicity Normal - Command Dword 11
*/
typedef	struct {
	UINT32	DN	: 1;	/* Disable Normal */
	UINT32	rsvd1	: 31;
} FEAT_WAN;

/*
** Figure 119: Asynchronous Event Configuration - Command Dword 11
*/
typedef	struct {
	UINT32	SCW	: 8;	/* SMART / Health Critical Warnings */
	UINT32	NAN	: 1;	/* Namespace Attribute Notices */
	UINT32	FAN	: 1;	/* Firmware Activation Notices */
	UINT32	rsvd10	: 22;
} FEAT_AEC;

/*
** Figure 120: Autonomous Power State Transition - Command Dword 11
*/
typedef	struct {
	UINT32	APSTE	: 1;	/* Autonomous Power State Transition Enable */
	UINT32	rsvd1	: 31;
} FEAT_APST;

#endif	/* _NVME_FEATURES_H */

