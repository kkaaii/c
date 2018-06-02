/******************************************************************************
** File Name: nvme_sq_entry.h
** Author:
** Creation Time: Wed Jan  3 23:03:53 2018
*/
#ifndef	_NVME_SQ_ENTRY_H
#define	_NVME_SQ_ENTRY_H

/*
** 4.2 Submission Queue Entry - Command Format
**
** Each command is 64 bytes in size.
**
** Command Dword 0, Namespace Identifier, Metadata Pointer, PRP Entry 1, PRP
** Entry 2, SGL Entry 1, and Metadata SGL Segment Pointer have common
** definitions for all Admin commands and NVM commands. Metadata Pointer, PRP
** Entry 1, PRP Entry 2, and Metadata SGL Segment Pointer are not used by all
** commands.
*/
typedef union {
	UINT32	all;
	struct {
		UINT32		OPC	: 8;	/* Opcode */
		UINT32		FUSE	: 2;	/* Fused Operation */
		UINT32		rsvd16	: 4;
		UINT32		PSDT	: 2;	/* PRP or SGL for Data Transfer */
		UINT32		CID	: 16;	/* Command Identifier */
	};
} NVME_SQE_DW0;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_SQE_DW0));

typedef enum {
	eSTC_SHORT		= 0x1,	/* Start a short device self-test operation */
	eSTC_EXTENDED		= 0x2,	/* Start an extended device self-test operation */
	eSTC_VS			= 0xE,	/* Vendor specific */
	eSTC_ABORT		= 0xF	/* Abort device self-test operation */
} SELFTEST_CODE;

typedef enum {
	eCA_UPDATE_FS		= 0x0,	/* Downloaded image replaces the
					image specified by the Firmware Slot
					field. This image is not activated. */

	eCA_UPDATE_FS_RST	= 0x1,	/* Downloaded image replaces the image
					specified by the Firmware Slot field.
					This image is activated at the next
					reset. */

	eCA_ACTIVATE_RST	= 0x2,	/* The image specified by the Firmware
					Slot field is activated at the next
					reset. */

	eCA_ACTIVATE_NOW	= 0x3,	/* The image specified by the Firmware
					Slot field is requested to be activated
					immediately without reset. */

	eCA_UPDATE_BP		= 0x6,	/* Downloaded image replaces the Boot
					Partition specified by the Boot
					Partition ID field. */

	eCA_ACTIVATE_BP_RST	= 0x7	/* Mark the Boot Partition specified
					in the BPID field as active and update
					BPINFO.ABPID */
} COMMIT_ACTION;

typedef union {
	UINT32		val;

	/*
	** Figure 43: Abort - Command Dword 10
	*/
	struct {
		UINT32	SQID	: 16;	/* Submission Queue Identifier */
		UINT32	CID	: 16;	/* Command Identifier */
	} abort;

	/*
	** Figure 52: Create I/O Completion Queue - Command Dword 10
	*/
	struct {
		UINT32	QID	: 16;	/* Queue Identifier */
		UINT32	QSIZE	: 16;	/* Queue Size */
	} createIoCompletionQueue;

	/*
	** Figure 56: Create I/O Submission Queue - Command Dword 10
	*/
	struct {
		UINT32	QID	: 16;	/* Queue Identifier */
		UINT32	QSIZE	: 16;	/* Queue Size */
	} createIoSubmissionQueue;

	/*
	** Figure 59: Delete I/O Completion Queue - Command Dword 10
	*/
	struct {
		UINT32	QID	: 16;	/* Queue Identifier */
		UINT32	rsvd16	: 16;
	} deleteIoCompletionQueue;

	/*
	** Figure 61: Delete I/O Submission Queue - Command Dword 10
	*/
	struct {
		UINT32	QID	: 16;	/* Queue Identifier */
		UINT32	rsvd16	: 16;
	} deleteIoSubmissionQueue;

	/*
	** Figure 67: Device Self-test - Command Dword 10
	*/
	struct {
		UINT32	STC	: 4;	/* Self-test Code */
		UINT32	rsvd4	: 28;
	} deviceSelfTest;

	/*
	** Figure 76: Firmware Commit - Command Dword 10
	*/
	struct {
		UINT32	FS	: 3;	/* Firmware Slot */
		UINT32	CA	: 3;	/* Commit Action */
		UINT32	rsvd8	: 25;
		UINT32	BPID	: 1;	/* Boot Partition ID */
	} firmwareCommit;

	/*
	** Figure 79: Firmware Image Download - Command Dword 10
	*/
	struct {
		UINT32	NUMD;		/* Number of Dwords */
	} firmwareImageDownload;

	/*
	** Figure 83: Get Features - Command Dword 10
	*/
	struct {
		UINT32	FID	: 8;	/* Feature Identifier */
		UINT32	SEL	: 3;	/* Select */
		UINT32	rsvd11	: 21;
	} getFeatures;

	/*
	** Figure 86: Get Log Page - Command Dword 10
	*/
	struct {
		UINT32	LID	: 8;	/* Log Page Identifier */
		UINT32	LSP	: 4;	/* Log Specific Field */
		UINT32	rsvd12	: 3;
		UINT32	RAE	: 1;	/* Retain Asynchronous Event */
		UINT32	NUMDL	: 16;	/* Number of Dwords Lower */
	} getLogPage;

	/*
	** Figure 108: Identify - Command Dword 10
	*/
	struct {
		UINT32	CNS	: 8;	/* Controller or Namespace Structure */
		UINT32	rsvd	: 8;
		UINT32	CNTID	: 16;	/* Controller Identifier */
	} identify;

	/*
	** Figure 133: Set Features - Command Dword 10
	*/
	struct {
		UINT32	FID	: 8;	/* Feature Identifier */
		UINT32	rsvd8	: 23;
		UINT32	SV	: 1;	/* Save */
	} setFeatures;
} NVME_SQE_DW10;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_SQE_DW10));

typedef union {
	UINT32			val;

	/*
	** Figure 53: Create I/O Completion Queue - Command Dword 11
	*/
	struct {
		UINT32	PC	: 1;	/* 00:00 Physically Contiguous */
		UINT32	IEN	: 1;	/* 01:01 Interrupts Enabled */
		UINT32	rsvd2	: 14;	/* 15:02 reserved */
		UINT32	IV	: 16;	/* 31:16 Interrupt Vector */
	} createIoCompletionQueue;

	/*
	** Figure 57: Create I/O Submission Queue - Command Dword 11
	*/
	struct {
		UINT32	PC	: 1;	/* 00:00 Physically Contiguous */
		UINT32	QPRIO	: 2;	/* 02:01 Queue Priority */
		UINT32	rsvd	: 13;	/* 15:03 reserved */
		UINT32	CQID	: 16;	/* 31:16 Completion Queue ID */
	} createIoSubmissionQueue;

	/*
	** Figure 80: Firmware Image Download - Command Dword 11
	*/
	struct {
		UINT32	OFST;		/* Offset */
	} firmwareImageDownload;

	/*
	** Figure 87: Get Log Page - Command Dword 11
	*/
	struct {
		UINT32	NUMDU	: 16;	/* Number of Dwords Upper */
		UINT32	rsvd16	: 16;
	} getLogPage;
} NVME_SQE_DW11;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_SQE_DW11));

typedef union {
	UINT32			val;

	/*
	** Figure 88: Get Log Page - Command Dword 12
	*/
	struct {
		UINT32	LPOL;		/* Log Page Offset Lower */
	} getLogPage;
} NVME_SQE_CDW12;

typedef union {
	UINT32			val;

	/*
	** Figure 89: Get Log Page - Command Dword 13
	*/
	struct {
		UINT32		LPOU;	/* Log Page Offset Upper */
	} getLogPage;
} NVME_SQE_CDW13;

typedef union {
	UINT32	val;
} NVME_CDW14;

typedef union {
	UINT32	val;
} NVME_CDW15;

/*
** Figure 11: Command Format - Amdin and NVM Command Set
*/
typedef struct {
	UINT64	PRP1;	/* PRP Entry 1 */
	UINT64	PRP2;	/* PRP Entry 2 */
} NVME_SQE_DPTR;

typedef struct {
	NVME_SQE_DW0	CDW0;		/* 03:00 Command Dword 0 (CDW0) */
	NVME_NSID	NSID;		/* 07:04 Namespace Identifier (NSID) */
	UINT32		rsvd[2];	/* 15:08 reserved */
	UINT64		mptr;		/* 23:16 Metadata Pointer (MPTR) */
	NVME_SQE_DPTR	DPTR;		/* 39:24 Data Pointer */
	NVME_SQE_DW10	CDW10;		/* 43:40 Command Dword 10 (CDW10) */
	NVME_SQE_DW11	CDW11;		/* 47:44 Command Dword 11 (CDW11) */
	NVME_SQE_CDW12	CDW12;		/* 51:48 Command Dword 12 (CDW12) */
	NVME_SQE_CDW13	CDW13;		/* 55:52 Command Dword 13 (CDW13) */
	NVME_CDW14	cdw14;		/* 59:56 Command Dword 14 (CDW14) */
	NVME_CDW15	cdw15;		/* 63:60 Command Dword 15 (CDW15) */
} NVME_SQE;

CC_ASSERT(64 == sizeof (NVME_SQE));

typedef	enum {
/*
** Figure 41: Opcodes for Admin Commands
*/
	NVME_OPC_ADMIN_DELETE_IOSQ	= 0x00,
	NVME_OPC_ADMIN_CREATE_IOSQ	= 0x01,
	NVME_OPC_ADMIN_GET_LOG_PAGE	= 0x02,
	NVME_OPC_ADMIN_DELETE_IOCQ	= 0x04,
	NVME_OPC_ADMIN_CREATE_IOCQ	= 0x05,
	NVME_OPC_ADMIN_IDENTIFY		= 0x06,
	NVME_OPC_ADMIN_ABORT		= 0x08,
	NVME_OPC_ADMIN_SET_FEATURES	= 0x09,
	NVME_OPC_ADMIN_GET_FEATURES	= 0x0A,
	NVME_OPC_ADMIN_AER		= 0x0C,
	NVME_OPC_ADMIN_NS_MANAGEMENT	= 0x0D,
	NVME_OPC_ADMIN_FW_COMMIT	= 0x10,
	NVME_OPC_ADMIN_FW_DOWNLOAD	= 0x11,
	NVME_OPC_ADMIN_DST		= 0x14,
	NVME_OPC_ADMIN_NS_ATTACHMENT	= 0x15,
	NVME_OPC_ADMIN_KEEP_ALIVE	= 0x18,
	NVME_OPC_ADMIN_DIRECTIVE_SND	= 0x19,
	NVME_OPC_ADMIN_DIRECTIVE_RCV	= 0x1A,
	NVME_OPC_ADMIN_VIRT_MGMT	= 0x1C,
	NVME_OPC_ADMIN_NVME_MI_SND	= 0x1D,
	NVME_OPC_ADMIN_NVME_MI_RCV	= 0x1E,
	NVME_OPC_ADMIN_DB_BUFFER_CONFIG	= 0x7C,

/*
** Figure 42: Opcodes for Admin Commands - NVM Commans Set Specific
*/
	NVME_OPC_ADMIN_FORMAT_NVM	= 0x80,
	NVME_OPC_ADMIN_SECURITY_SND	= 0x81,
	NVME_OPC_ADMIN_SECURITY_RCV	= 0x82,
	NVME_OPC_ADMIN_SANITIZE		= 0x84,

/*
** Figure 188: Opcodes for NVM Commands
*/
	NVME_OPC_IO_FLUSH		= 0x00,
	NVME_OPC_IO_WRITE		= 0x01,
	NVME_OPC_IO_READ		= 0x02,
	NVME_OPC_IO_WRITE_UNC		= 0x04,
	NVME_OPC_IO_COMPARE		= 0x05,
	NVME_OPC_IO_WRITE_ZEROES	= 0x08,
	NVME_OPC_IO_DATASET_MGMT	= 0x09,
	NVME_OPC_IO_RESERVATION_REG	= 0x0D,
	NVME_OPC_IO_RESERVATION_REPORT	= 0x0E,
	NVME_OPC_IO_RESERVATION_ACQUIRE	= 0x11,
	NVME_OPC_IO_RESERVATION_RELEASE	= 0x15,

	NVME_OPC_IO_VU_START		= 0x80

} NVME_OPCODE;

#endif	/* _NVME_SQ_ENTRY_H */

