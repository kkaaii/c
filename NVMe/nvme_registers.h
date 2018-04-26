#ifndef _NVME_REGISTERS_H
#define	_NVME_REGISTERS_H

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
} NVME_REG64_00H;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_00H));

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
} NVME_REG32_08H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_08H));

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
} NVME_REG32_14H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_14H));

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
} NVME_REG32_1CH;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_1CH));

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
} NVME_REG32_24H;

CC_ASSERT(sizeof(UINT32) == sizeof (NVME_REG32_24H));

/*
** 3.1.9 Offset 28h: ASQ - Admin Submission Queue Base Address
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;
		UINT64	ASQB	: 52;	/* Admin Submission Queue Base */
	};
} NVME_REG64_28H;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_28H));

/*
** 3.1.10 Offset 30h: ACQ - Admin Completion Queue Base Address
*/
typedef union {
	UINT64	reg;
	struct {
		UINT64	rsvd0	: 12;
		UINT64	ACQB	: 52;	/* Admin Completion Queue Base */
	};
} NVME_REG64_30H;

CC_ASSERT(sizeof(UINT64) == sizeof (NVME_REG64_30H));

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
** 3.1 Register Definition
*/
typedef struct {
	NVME_REG64_00H	CAP;
	NVME_REG32_08H	VS;
	UINT32		INTMS;
	UINT32		INTMC;
	NVME_REG32_14H	CC;
	UINT32		rsvd18h;
	NVME_REG32_1CH	CSTS;
	UINT32		NSSR;
	NVME_REG32_24H	AQA;
	NVME_REG64_28H	ASQ;
	NVME_REG64_30H	ACQ;
	NVME_REG32_38H	CMBLOC;
	NVME_REG32_3CH	CMBSZ;
	NVME_REG32_40H	BPINFO;
	NVME_REG32_44H	BPRSEL;
	NVME_REG64_48H	BPMBL;
} NVME_CONFIG;

#endif	/* NVME_REGISTERS_H */

