#ifndef	_GET_LOGPAGE_H
#define	_GET_LOGPAGE_H

/*
** Figure 77: Get Log Page - Log Page Identifiers
*/
typedef enum {
	eLID_Reserved_00h			= 0x00,
	eLID_ErrorInformation			= 0x01,
	eLID_SmartHealthInformation		= 0x02,
	eLID_FirmwareSlotInformation		= 0x03,
	eLID_ChangedNamespaceList		= 0x04,
	eLID_CommandEffectsLog			= 0x05,
	eLID_DeviceSelfTest			= 0x06,	/* 1.3 - */
	eLID_HostInitiatedTelemetry		= 0x07,	/* 1.3 - */
	eLID_ControllerInitiatedTelemetry	= 0x08,	/* 1.3 - */

	eLID_ReservationNotification		= 0x80,
	eLID_SanitizeStatus			= 0x81	/* 1.3 - */
} NVME_LID;

#endif	/* _GET_LOGPAGE_H */

