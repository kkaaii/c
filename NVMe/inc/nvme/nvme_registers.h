#ifndef _NVME_CONTROLLER_H
#define	_NVME_CONTROLLER_H

/*
** 3.1.1 Offset 00h: CAP - Controller Capabilities
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	MQES	: 16;	/* 15:00 (RO) Maximum Queue Entries Supported */

		UINT64	CQR	: 1;	/* 16:16 (RO) Contiguous Queues Required */
		UINT64	AMS	: 2;	/* 18:17 (RO) Arbitration Mechanism Supported */
		UINT64	rsvd19	: 5;	/* 23:19 (RO) */
		UINT64	TO	: 8;	/* 31:24 (RO) Timeout */

		UINT64	DSTRD	: 4;	/* 35:32 (RO) Doorbell Stride */
		UINT64	NSSRS	: 1;	/* 36:36 (RO) NVM Subsystem Reset Supported */
		UINT64	CSS	: 8;	/* 44:37 (RO) Command Sets Supported */
		UINT64	BPS	: 1;	/* 45:45 (RO) Boot Partition Support */
		UINT64	rsvd46	: 2;	/* 47:46 (RO) */

		UINT64	MPSMIN	: 4;	/* 51:48 (RO) Memory Page Size Minimum */
		UINT64	MPSMAX	: 4;	/* 55:52 (RO) Memory Page Size Maximum */
		UINT64	rsvd56	: 8;	/* 63:56 (RO) */
	};
} NVME_REG64_CAP;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_CAP));

/*
** 3.1.2 Offset 08h: VS - Version
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	TER	: 8;	/* 07:00 (RO) Tertiary Version Number */
		UINT32	MNR	: 8;	/* 15:08 (RO) Minor Version Number */

		UINT32	MJR	: 16;	/* 31:16 (RO) Major Version Number */
	};
} NVME_REG32_VS;

enum {
	eVS_1_0		= 0x00010000,	/* VS Value for 1.0 Compliant Controllers */
	eVS_1_1		= 0x00010100,	/* VS Value for 1.1 Compliant Controllers */
	eVS_1_2		= 0x00010200,	/* VS Value for 1.2 Compliant Controllers */
	eVS_1_2_1	= 0x00010201,	/* VS Value for 1.2.1 Compliant Controllers */
	eVS_1_3		= 0x00010300,	/* VS Value for 1.3 Compliant Controllers */
};

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_VS));

/*
** 3.1.5 Offset 14h: CC - Controller Configuration
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	EN	: 1;	/* 00:00 (RW) Enable */
		UINT32	rsvd1	: 3;	/* 03:01 (RO) */
		UINT32	CSS	: 3;	/* 06:04 (RW) I/O Command Set Selected */
		UINT32	MPS	: 4;	/* 10:07 (RW) Memory Page Size */
		UINT32	AMS	: 3;	/* 13:11 (RW) Arbitration Mechanism Selected */
		UINT32	SHN	: 2;	/* 15:14 (RW) Shutdown Notification */

		UINT32	IOSQES	: 4;	/* 19:16 (RW) I/O Submission Queue Entry Size */
		UINT32	IOCQES	: 4;	/* 23:20 (RW) I/O Completion Queue Entry Size */
		UINT32	rsvd24	: 8;	/* 31:24 (RO) */
	};
} NVME_REG32_CC;

enum {
	eSHN_NoNotification		= 0,
	eSHN_NormalShutdownNotification	= 1,
	eSHN_AbruptShutdownNotification	= 2,
	eSHN_Reserved_3			= 3,
};

enum {
	eAMS_RoundRobin					= 0,
	eAMS_WeightedRoundRobinWithUrgentPriorityClass	= 1,
	eAMS_Reserved_2					= 2,
	eAMS_Reserved_3					= 3,
	eAMS_Reserved_4					= 4,
	eAMS_Reserved_5					= 5,
	eAMS_Reserved_6					= 6,
	eAMS_VendorSpecific				= 7,
};

enum {
	eCSS_NvmCommandSet	= 0,
	eCSS_Reserved_1		= 1,
	eCSS_Reserved_2		= 2,
	eCSS_Reserved_3		= 3,
	eCSS_Reserved_4		= 4,
	eCSS_Reserved_5		= 5,
	eCSS_Reserved_6		= 6,
	eCSS_Reserved_7		= 7,
};

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_CC));

/*
** 3.1.6 Offset 1Ch: CSTS - Controller Status
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	RDY	: 1;	/* 00:00 (RO) Ready */
		UINT32	CFS	: 1;	/* 01:01 (RO) Controller Fatal Status */
		UINT32	SHST	: 2;	/* 03:02 (RO) Shutdown Status */
		UINT32	NSSRO	: 1;	/* 04:04 (RW1C) NVM Subsystem Reset Occurred */
		UINT32	PP	: 1;	/* 05:05 (RO) Processing Paused */
		UINT32	rsvd6	: 26;	/* 31:06 (RO) */
	};
} NVME_REG32_CSTS;

enum {
	eSHST_NormalOperation			= 0,
	eSHST_ShutdownProcessingOccurring	= 1,
	eSHST_ShutdownProcessingComplete	= 2,
	eSHST_Reserved_3			= 3,
};

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_CSTS));

/*
** 3.1.7 Offset 20h: NSSR - NVM Subsystem Reset
*/
enum {
	eNSSRC_Reset	= 0x4E564D65,	/* "NVMe" */
};

/*
** 3.1.8 Offset 24h: AQA - Admin Queue Attributes
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	ASQS	: 12;	/* 11:00 (RW) Admin Submission Queue Size */
		UINT32	rsvd12	: 4;	/* 15:12 (RO) */

		UINT32	ACQS	: 12;	/* 27:16 (RW) Admin Completion Queue Size */
		UINT32	rsvd28	: 4;	/* 31:28 (RO) */
	};
} NVME_REG32_AQA;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_AQA));

/*
** 3.1.9 Offset 28h: ASQ - Admin Submission Queue Base Address
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;	/* 11:00 (RO) */
		UINT64	ASQB	: 52;	/* 63:12 (RW) Admin Submission Queue Base */
	};
} NVME_REG64_ASQ;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_ASQ));

/*
** 3.1.10 Offset 30h: ACQ - Admin Completion Queue Base Address
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;	/* 11:00 (RO) */
		UINT64	ACQB	: 52;	/* 63:12 (RW) Admin Completion Queue Base */
	};
} NVME_REG64_ACQ;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_ACQ));

/*
** 3.1.11 Offset 38h: CMBLOC - Controller Memory Buffer Location
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	BIR	: 3;	/* 02:00 (RO) Base Indicator Register */
		UINT32	rsvd3	: 9;	/* 11:03 (RO) */
		UINT32	OFSET	: 20;	/* 31:12 (RO) Offset */
	};
} NVME_REG32_38H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_38H));

/*
** 3.1.12 Offset 3Ch: CMBSZ - Controller Memory Buffer Size
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	SQS	: 1;	/* 00:00 (RO) Submission Queue Support */
		UINT32	CQS	: 1;	/* 01:01 (RO) Completion Queue Support */
		UINT32	LISTS	: 1;	/* 02:02 (RO) PRP SGL List Support */
		UINT32	RDS	: 1;	/* 03:03 (RO) Read Data Support */
		UINT32	WDS	: 1;	/* 04:04 (RO) Write Data Support */
		UINT32	rsvd5	: 3;	/* 07:05 (RO) */
		UINT32	SZU	: 4;	/* 11:08 (RO) Size Units */
		UINT32	SZ	: 20;	/* 31:12 (RO) Size */
	};
} NVME_REG32_3CH;

enum {
	eSZU_4KB		= 0x0,
	eSZU_64KB		= 0x1,
	eSZU_1MB		= 0x2,
	eSZU_16MB		= 0x3,
	eSZU_256MB		= 0x4,
	eSZU_4GB		= 0x5,
	eSZU_64GB		= 0x6,
	eSZU_Reserved_7h	= 0x7,
	eSZU_Reserved_8h	= 0x8,
	eSZU_Reserved_9h	= 0x9,
	eSZU_Reserved_Ah	= 0xA,
	eSZU_Reserved_Bh	= 0xB,
	eSZU_Reserved_Ch	= 0xC,
	eSZU_Reserved_Dh	= 0xD,
	eSZU_Reserved_Eh	= 0xE,
	eSZU_Reserved_Fh	= 0xF,
};

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_3CH));

/*
** 3.1.13 Offset 40h: BPINFO - Boot Partition Information
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	BPSZ	: 15;	/* 14:00 (RO) Boot Partition Size */
		UINT32	rsvd15	: 9;	/* 23:15 (RO) */
		UINT32	BRS	: 2;	/* 25:24 (RO) Boot Read Status */
		UINT32	rsvd26	: 5;	/* 30:26 (RO) */
		UINT32	ABPID	: 1;	/* 31:31 (RO) Active Boot Partition ID */
	};
} NVME_REG32_40H;

enum {
	eBRS_NoBootPartitionReadOperationRequested	= 0,
	eBRS_BootPartitionReadInProgress		= 1,
	eBRS_BootPartitionReadCompletedSuccessfully	= 2,
	eBRS_ErrorCompletingBootPartitionRead		= 3,
};

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_40H));

/*
** 3.1.14 Offset 44h: BPRSEL - Boot Partition Read Select
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	BPRSZ	: 10;	/* 09:00 (RW) Boot Partition Read Size */
		UINT32	BPROF	: 20;	/* 29:00 (RW) Boot Partition Read Offset */
		UINT32	rsvd30	: 1;	/* 30:30 (RO) */
		UINT32	BPID	: 1;	/* 31:31 (RW) Boot Partition Identifier */
	};
} NVME_REG32_44H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_44H));

/*
** 3.1.15 Offset 48h: BPMBL - Boot Partition Memory Buffer Location (Optional)
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;	/* 11:00 (RO) */
		UINT64	BMBBA	: 52;	/* 63:12 (RW) Boot Partition Memory Buffer Base Address */
	};
} NVME_REG64_48H;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_48H));

/*
** 3.1.16 Offset 1000h + ((2y) * (4 << CAP.DSTRD)): SQyTDBL - Submission Queue y Tail Doorbell
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	SQT	: 16;	/* 15:00 (RW) Submission Queue Tail */
		UINT32	rsvd16	: 16;	/* 31:16 (RO) */
	};
} NVME_REG32_SQT;

/*
** 3.1.17 Offset 1000h + ((2y + 1) * (4 << CAP.DSTRD)): CQyHDBL - Completion Queue y Head Doorbell
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	CQH	: 16;	/* 15:00 (RW) Completion Queue Head */
		UINT32	rsvd16	: 16;	/* 31:16 (RO) */
	};
} NVME_REG32_CQH;

/*
** 3.1 Register Definition
*/
typedef struct {
	NVME_REG64_CAP	CAP;	/* 0007h:0000h Controller Capabilities */
	NVME_REG32_VS	VS;	/* 000Bh:0008h Version */
	UINT32		INTMS;	/* 000Fh:000Ch Interrupt Mask Set */
	UINT32		INTMC;	/* 0013h:0010h Interrupt Mask Clear */
	NVME_REG32_CC	CC;	/* 0017h:0014h Controller Configuration */
	UINT32		rsvd18h;
	NVME_REG32_CSTS	CSTS;	/* 001Fh:001Ch Controller Status */
	UINT32		NSSR;	/* 0023h:0020h NVM Subsystem Reset */
	NVME_REG32_AQA	AQA;	/* 0027h:0024h Admin Queue Attributes */
	NVME_REG64_ASQ	ASQ;	/* 002Fh:0028h Admin Submission Queue Base Address */
	NVME_REG64_ACQ	ACQ;	/* 0037h:0030h Admin Completion Queue Base Address */
	NVME_REG32_38H	CMBLOC;	/* 003Bh:0038h Controller Memory Buffer Location */
	NVME_REG32_3CH	CMBSZ;	/* 003Fh:003Ch Controller Memory Buffer Size */
	NVME_REG32_40H	BPINFO;	/* 0043h:0040h Boot Partition Information */
	NVME_REG32_44H	BPRSEL;	/* 0047h:0044h Boot Partition Read Select */
	NVME_REG64_48H	BPMBL;	/* 004Fh:0048h Boot Partition Memory Buffer Location */
} NVME_CONTROLLER;

#endif	/* NVME_CONTROLLER_H */

