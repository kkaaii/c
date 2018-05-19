#ifndef _NVME_CONTROLLER_H
#define	_NVME_CONTROLLER_H

/*
** 3.1.1 Offset 00h: CAP - Controller Capabilities
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	MQES	: 16;	/* Maximum Queue Entries Supported */
		UINT64	CQR	: 1;	/* Contiguous Queues Required */
		UINT64	AMS	: 2;	/* Arbitration Mechanism Supported */
		UINT64	rsvd19	: 5;
		UINT64	TO	: 8;	/* Timeout */
		UINT64	DSTRD	: 4;	/* Doorbell Stride */
		UINT64	NSSRS	: 1;	/* NVM Subsystem Reset Supported */
		UINT64	CSS	: 8;	/* Command Sets Supported */
		UINT64	BPS	: 1;	/* Boot Partition Support */
		UINT64	rsvd46	: 2;
		UINT64	MPSMIN	: 4;	/* Memory Page Size Minimum */
		UINT64	MPSMAX	: 4;	/* Memory Page Size Maximum */
		UINT64	rsvd56	: 8;
	};
} NVME_REG64_CAP;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_CAP));

/*
** 3.1.2 Offset 08h: VS - Version
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	rsvd0	: 8;
		UINT32	MNR	: 8;	/* Minor Version Number */
		UINT32	MJR	: 16;	/* Major Version Number */
	};
} NVME_REG32_VS;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_VS));

#define	VS_1_0		0x00010000	/* VS Value for 1.0 Compliant Controllers */
#define	VS_1_1		0x00010100	/* VS Value for 1.1 Compliant Controllers */
#define	VS_1_2		0x00010200	/* VS Value for 1.2 Compliant Controllers */
#define	VS_1_2_1	0x00010201	/* VS Value for 1.2.1 Compliant Controllers */
#define	VS_1_3		0x00010300	/* VS Value for 1.3 Compliant Controllers */

/*
** 3.1.5 Offset 14h: CC - Controller Configuration
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	EN	: 1;	/* Enable */
		UINT32	rsvd1	: 3;
		UINT32	CSS	: 3;	/* I/O Command Set Selected */
		UINT32	MPS	: 4;	/* Memory Page Size */
		UINT32	AMS	: 3;	/* Arbitration Mechanism Selected */
		UINT32	SHN	: 2;	/* Shutdown Notification */
		UINT32	IOSQES	: 4;	/* I/O Submission Queue Entry Size */
		UINT32	IOCQES	: 4;	/* I/O Completion Queue Entry Size */
		UINT32	rsvd24	: 8;
	};
} NVME_REG32_CC;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_CC));

#define	CC_SHN_NO_NOTIFICATION	0
#define	CC_SHN_NORMAL_SHUTDOWN	1
#define	CC_SHN_ABRUPT_SHUTDOWN	2
#define	CC_SHN_RESERVED		3

#define	CC_AMS_ROUND_ROBIN	0
#define	CC_AMS_WRR_UPS		1	/* Weighted Round Robin with Urgent Priority Class */
#define	CC_AMS_VENDOR_SPECIFIC	7

#define	CC_CSS_NVM_COMMAND_SET	0

/*
** 3.1.6 Offset 1Ch: CSTS - Controller Status
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	RDY	: 1;	/* Ready */
		UINT32	CFS	: 1;	/* Controller Fatal Status */
		UINT32	SHST	: 2;	/* Shutdown Status */
		UINT32	NSSRO	: 1;	/* NVM Subsystem Reset Occurred */
		UINT32	PP	: 1;	/* Processing Paused */
		UINT32	rsvd6	: 26;
	};
} NVME_REG32_CSTS;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_CSTS));

#define	CSTS_SHST_NORMAL_OPERATION	0
#define	CSTS_SHST_SHUTDOWN_OCCURRING	1
#define	CSTS_SHST_SHUTDOWN_COMPLETE	2
#define	CSTS_SHST_RESERVED		3

/*
** 3.1.8 Offset 24h: AQA - Admin Queue Attributes
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	ASQS	: 12;	/* Admin Submission Queue Size */
		UINT32	rsvd12	: 4;
		UINT32	ACQS	: 12;	/* Admin Completion Queue Size */
		UINT32	rsvd28	: 4;
	};
} NVME_REG32_AQA;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_AQA));

/*
** 3.1.9 Offset 28h: ASQ - Admin Submission Queue Base Address
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;
		UINT64	ASQB	: 52;	/* Admin Submission Queue Base */
	};
} NVME_REG64_ASQ;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_ASQ));

/*
** 3.1.10 Offset 30h: ACQ - Admin Completion Queue Base Address
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;
		UINT64	ACQB	: 52;	/* Admin Completion Queue Base */
	};
} NVME_REG64_ACQ;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_ACQ));

/*
** 3.1.11 Offset 38h: CMBLOC - Controller Memory Buffer Location
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	BIR	: 3;	/* Base Indicator Register */
		UINT32	rsvd3	: 9;
		UINT32	OFSET	: 20;	/* Offset */
	};
} NVME_REG32_38H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_38H));

/*
** 3.1.12 Offset 3Ch: CMBSZ - Controller Memory Buffer Size
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	SQS	: 1;	/* Submission Queue Support */
		UINT32	CQS	: 1;	/* Completion Queue Support */
		UINT32	LISTS	: 1;	/* PRP SGL List Support */
		UINT32	RDS	: 1;	/* Read Data Support */
		UINT32	WDS	: 1;	/* Write Data Support */
		UINT32	rsvd5	: 3;
		UINT32	SZU	: 4;	/* Size Units */
		UINT32	SZ	: 20;	/* Size */
	};
} NVME_REG32_3CH;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_3CH));

#define	CMBSZ_SZU_4KB	0
#define	CMBSZ_SZU_64KB	1
#define	CMBSZ_SZU_1MB	2
#define	CMBSZ_SZU_16MB	3
#define	CMBSZ_SZU_256MB	4
#define	CMBSZ_SZU_4GB	5
#define	CMBSZ_SZU_64GB	6

/*
** 3.1.13 Offset 40h: BPINFO - Boot Partition Information
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	BPSZ	: 15;	/* Boot Partition Size */
		UINT32	rsvd15	: 9;
		UINT32	BRS	: 2;	/* Boot Read Status */
		UINT32	rsvd26	: 5;
		UINT32	ABPID	: 1;	/* Active Boot Partition ID */
	};
} NVME_REG32_40H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_40H));

#define	BPINFO_BRS_NO_READ		0
#define	BPINFO_BRS_READ_IN_PROGRESS	1
#define	BPINFO_BRS_READ_SUCCESSFULLY	2
#define	BPINFO_BRS_READ_ERROR		3

/*
** 3.1.14 Offset 44h: BPRSEL - Boot Partition Read Select
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	BPRSZ	: 10;	/* Boot Partition Read Size */
		UINT32	BPROF	: 20;	/* Boot Partition Read Offset */
		UINT32	rsvd30	: 1;
		UINT32	BPID	: 1;	/* Boot Partition Identifier */
	};
} NVME_REG32_44H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_44H));

/*
** 3.1.15 Offset 48h: BPMBL - Boot Partition Memory Buffer Location (Optional)
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;
		UINT64	BMBBA	: 52;	/* Boot Partition Memory Buffer Base Address */
	};
} NVME_REG64_48H;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_48H));

/*
** 3.1.16 Offset 1000h + ((2y) * (4 << CAP.DSTRD)): SQyTDBL - Submission Queue y Tail Doorbell
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	SQT	: 16;	/* Submission Queue Tail */
		UINT32	rsvd16	: 16;
	};
} NVME_REG32_SQT;

/*
** 3.1.17 Offset 1000h + ((2y + 1) * (4 << CAP.DSTRD)): CQyHDBL - Completion Queue y Head Doorbell
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	CQH	: 16;	/* Completion Queue Head */
		UINT32	rsvd16	: 16;
	};
} NVME_REG32_CQH;

/*
** 3.1 Register Definition
*/
typedef struct {
	NVME_REG64_CAP	CAP;
	NVME_REG32_VS	VS;
	UINT32		INTMS;
	UINT32		INTMC;
	NVME_REG32_CC	CC;
	UINT32		rsvd18h;
	NVME_REG32_CSTS	CSTS;
	UINT32		NSSR;
	NVME_REG32_AQA	AQA;
	NVME_REG64_ASQ	ASQ;
	NVME_REG64_ACQ	ACQ;
	NVME_REG32_38H	CMBLOC;
	NVME_REG32_3CH	CMBSZ;
	NVME_REG32_40H	BPINFO;
	NVME_REG32_44H	BPRSEL;
	NVME_REG64_48H	BPMBL;
} NVME_CONTROLLER;

#endif	/* NVME_CONTROLLER_H */

