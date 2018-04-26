#ifndef _NVME_REGISTERS_H
#define	_NVME_REGISTERS_H

/*
** 3.1.8 Offset 24h: AQA - Admin Queue Attributes
*/
typedef union {
	UINT32	reg;
	struct {
		UINT32	ASQS : 12;	/* Admin Submission Queue Size */
		UINT32	rsv0 : 4;
		UINT32	ACQS : 12;	/* Admin Completion Queue Size */
		UINT32	rsv1 : 4;
	};
} NVME_REG_24H;

/*
** 3.1 Register Definition
*/
typedef struct {
	UINT64	CAP;	/* 00h - Controller Capabilities */
	UINT32	VS;	/* 08h - Version */
	UINT32	INTMS;	/* 0Ch - Interrupt Mask Set */
	UINT32	INTMC;	/* 10h - Interrupt Mask Clear */
	UINT32	CC;	/* 14h - Controller Configuration */
	UINT32	rsvd0;
	UINT32	CSTS;	/* 1Ch - Controller Status */
	UINT32	NSSR;	/* 20h - NVM Subsystem Reset (Optional) */
	NVME_REG_24H	AQA;	/* 24h - Admin Queue Attributes */
	UINT64	ASQ;	/* 28h - Admin Submission Queue Base Address */
	UINT64	ACQ;	/* 30h - Admin Completion Queue base Address */
	UINT32	CMBLOC;	/* 38h - Controller Memory Buffer Location (Optional) */
	UINT32	CMBSZ;	/* 3Ch - Controller Memory Buffer Size (Optional) */
	UINT32	BPINFO;	/* 40h - Boot Partition Information (Optional) */
	UINT32	BPRSEL;	/* 44h - Boot Partition Read Select (Optional) */
	UINT64	BPMBL;	/* 48h - Boot Partition Memory Buffer Location (Optional) */
} NVME_CONFIG;

#endif	/* NVME_REGISTERS_H */

