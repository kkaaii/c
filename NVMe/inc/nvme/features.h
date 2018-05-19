#ifndef _NVME_FEATURES_H
#define	_NVME_FEATURES_H

/*
** Figure 70: Get Features - Command Dword 10
*/
typedef	enum {
	/* 000b */eSEL_Current,
	/* 001b */eSEL_Default,
	/* 010b */eSEL_Saved,
	/* 011b */eSEL_SupportedCapabilities
} NVME_SEL;

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

#endif	/* _NVME_FEATURES_H */

