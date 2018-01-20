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
typedef	struct {
	UINT32		opc	: 8;	/* Opcode (OPC) */
	UINT32		fuse	: 2;	/* Fused Operation (FUSE) */
	UINT32		rsvd	: 4;	/* reserved */
	UINT32		psdt	: 2;	/* PRP or SGL for Data Transfer (PSDT) */
	UINT32		cid	: 16;	/* Command Identifier (CID) */
} NVME_CDW0_FIELDS;

typedef union {
	UINT32			val;
	NVME_CDW0_FIELDS	fields;
} NVME_CDW0;

typedef struct {
	UINT64		prp1;		/* PRP Entry 1 (PRP1) */
	UINT64		prp2;		/* PRP Entry 2 (PRP2) */
} NVME_DPTR;

typedef struct {
	UINT16		sqid;		/* Submission Queue Identifier (SQID) */
	UINT16		cid;		/* Command Identifier (CID) */
} NVME_CDW10_ABORT;

typedef struct {
	UINT16		qid;		/* Queue Identifier (QID) */
	UINT16		qsize;		/* Queue Size (QSIZE) */
} NVME_CDW10_CREATEQ;

typedef struct {
	UINT16		qid;		/* Queue Identifier (QID) */
	UINT16		rsvd;		/* reserved */
} NVME_CDW10_DELETEQ;

typedef enum {
	eSTC_SHORT		= 0x1,	/* Start a short device self-test operation */
	eSTC_EXTENDED		= 0x2,	/* Start an extended device self-test operation */
	eSTC_VS			= 0xE,	/* Vendor specific */
	eSTC_ABORT		= 0xF	/* Abort device self-test operation */
} SELFTEST_CODE;

typedef struct {
	UINT32		stc	: 4;	/* Self-test Code (STC) */
	UINT32		rsvd	: 28;	/* reserved */
} NVME_CDW10_SELFTEST;

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

typedef struct {
	UINT32		fs	: 3;	/* Firmware Slot (FS) */
	UINT32		ca	: 3;	/* Commit Action (CA) */
	UINT32		rsvd	: 25;	/* reserved */
	UINT32		bpid	: 1;	/* Boot Partition ID (BPID) */
} NVME_CDW10_FWCOMMIT;

typedef struct {
	UINT32		numd;		/* Number of DWords (NUMD) */
} NVME_CDW10_NUMD;

typedef struct {
	UINT32		fid	: 8;	/* Feature Identifier (FID) */
	UINT32		sel	: 3;	/* Select (SEL) */
	UINT32		rsvd	: 21;	/* reserved */
} NVME_CDW10_GET_FEATURES;

typedef struct {
	UINT32		lid	: 8;	/* Log Page Identifier (LID) */
	UINT32		lsp	: 4;	/* Log Specific Field (LSP) */
	UINT32		rsvd	: 3;	/* reserved */
	UINT32		rae	: 1;	/* Retain Asynchronous Event (RAE) */
	UINT32		numdl	: 16;	/* Number of Dwords Lower (NUMDL) */
} NVME_CDW10_GET_LOG_PAGE;

typedef struct {
	UINT32		cns	: 8;	/* Controller or Namespace Structure (CNS) */
	UINT32		rsvd	: 8;	/* reserved */
	UINT32		cntid	: 16;	/* Controller Identifier (CNTID) */
} NVME_CDW10_IDENTIFY;

typedef struct {
	UINT32		fid	: 8;	/* Feature Identifier (FID) */
	UINT32		rsvd	: 23;	/* reserved */
	UINT32		sv	: 1;	/* Save (SV) */
} NVME_CDW10_SET_FEATURES;

typedef union {
	UINT32			val;
	NVME_CDW10_ABORT	abort;
	NVME_CDW10_CREATEQ	createq;
	NVME_CDW10_DELETEQ	deleteq;
	NVME_CDW10_SELFTEST	selftest;
	NVME_CDW10_FWCOMMIT	fwcommit;
	NVME_CDW10_NUMD		fwdownload;
	NVME_CDW10_GET_FEATURES	getFeatures;
	NVME_CDW10_GET_LOG_PAGE	getLogPage;
	NVME_CDW10_IDENTIFY	identify;
	NVME_CDW10_SET_FEATURES	setFeatures;
} NVME_CDW10;

typedef struct {
	UINT32		ofst;		/* Offset (OFST) */
} NVME_CDW11_OFST;

/*
** Figure 53: Create I/O Completion Queue - Command Dword 11
*/
typedef	struct {
	UINT16		pc	: 1;	/* 00:00 Physically Contiguous (PC) */
	UINT16		ien	: 1;	/* 01:01 Interrupts Enabled (IEN) */
	UINT16		rsvd	: 14;	/* 15:02 reserved */
	UINT16		iv;		/* 31:16 Interrupt Vector (IV) */
} NVME_CDW11_CREATE_IOCQ;

/*
** Figure 57: Create I/O Submission Queue - Command Dword 11
*/
typedef	struct {
	UINT16		pc	: 1;	/* 00:00 Physically Contiguous (PC) */
	UINT16		qprio	: 2;	/* 02:01 Queue Priority (QPRIO) */
	UINT16		rsvd	: 13;	/* 15:03 reserved */
	UINT16		cqid;		/* 31:16 Completion Queue ID (CQID) */
} NVME_CDW11_CREATE_IOSQ;

typedef struct {
	UINT16		numdu;		/* Number of Dwords Upper (NUMDU) */
	UINT16		rsvd;		/* reserved */
} NVME_CDW11_GET_LOG_PAGE;

typedef union {
	UINT32			val;
	NVME_CDW11_CREATE_IOCQ	createIoCq;
	NVME_CDW11_CREATE_IOSQ	createIoSq;
	NVME_CDW11_OFST		fwdownload;
	NVME_CDW11_GET_LOG_PAGE	getLogPage;
} NVME_CDW11;

typedef struct {
	UINT32		lpol;		/* Log Page Offset Lower (LPOL) */
} NVME_CDW12_GET_LOG_PAGE;

typedef union {
	UINT32			val;
	NVME_CDW12_GET_LOG_PAGE	getLogPage;
} NVME_CDW12;

typedef struct {
	UINT32		lpou;		/* Log Page Offset Upper (LPOU) */
} NVME_CDW13_GET_LOG_PAGE;

typedef union {
	UINT32			val;
	NVME_CDW13_GET_LOG_PAGE	getLogPage;
} NVME_CDW13;

typedef union {
	UINT32	val;
} NVME_CDW14;

typedef union {
	UINT32	val;
} NVME_CDW15;

typedef struct {
	NVME_CDW0	cdw0;		/* 03:00 Command Dword 0 (CDW0) */
	UINT32		nsid;		/* 07:04 Namespace Identifier (NSID) */
	UINT32		rsvd[2];	/* 15:08 reserved */
	UINT64		mptr;		/* 23:16 Metadata Pointer (MPTR) */
	NVME_DPTR	dptr;		/* 39:24 Data Pointer */
	NVME_CDW10	cdw10;		/* 43:40 Command Dword 10 (CDW10) */
	NVME_CDW11	cdw11;		/* 47:44 Command Dword 11 (CDW11) */
	NVME_CDW12	cdw12;		/* 51:48 Command Dword 12 (CDW12) */
	NVME_CDW13	cdw13;		/* 55:52 Command Dword 13 (CDW13) */
	NVME_CDW14	cdw14;		/* 59:56 Command Dword 14 (CDW14) */
	NVME_CDW15	cdw15;		/* 63:60 Command Dword 15 (CDW15) */
} NVME_SQ_ENTRY;

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

