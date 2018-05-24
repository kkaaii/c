#ifndef	_IDENTIFY_H
#define	_IDENTIFY_H

enum {
	eCNS_NamespaceData	= 0,
	eCNS_ControllerData	= 1,
	eCNS_ActiveNsidList	= 2
};

typedef struct PowerStateDescriptor {
	UINT32	MP	: 16;	/* 015:000 Maximum Power */
	UINT32	rsvd16	: 8;	/* 023:016 */
	UINT32	MXPS	: 1;	/* 024:024 Max Power Scale */
	UINT32	NOPS	: 1;	/* 025:025 Non-Operational State */
	UINT32	rsvd26	: 6;	/* 031:026 */

	UINT32	ENLAT;		/* 063:032 Entry Latency */

	UINT32	EXLAT;		/* 095:064 Exit Latency */

	UINT32	RRT	: 5;	/* 100:096 Relative Read Throughput */
	UINT32	rsvd101	: 3;	/* 103:101 */
	UINT32	RRL	: 5;	/* 108:104 Relative Read Latency */
	UINT32	rsvd109	: 3;	/* 111:109 */
	UINT32	RWT	: 5;	/* 116:112 Relative Write Throughput */
	UINT32	rsvd117	: 3;	/* 119:117 */
	UINT32	RWL	: 5;	/* 124:120 Relative Write Latency */
	UINT32 rsvd125	: 3;	/* 127:125 */

	UINT32	IDLP	: 16;	/* 143:128 Idle Power */
	UINT32	rsvd144	: 6;	/* 149:144 */
	UINT32	IPS	: 2;	/* 151:150 Idle Power Scale */
	UINT32	rsvd152	: 8;	/* 159:152 */

	UINT32	ACTP	: 16;	/* 175:160 Active Power */
	UINT32	APW	: 3;	/* 178:176 Active Power Workload */
	UINT32	rsvd179	: 3;	/* 181:179 */
	UINT32	APS	: 2;	/* 183:182 Active Power Scale */
	UINT32	rsvd184	: 8;	/* 191:184 */

	UINT32	rsvd192[2];	/* 255:192 */
} NVME_PSD;

CC_ASSERT(32 == sizeof (NVME_PSD));

typedef struct {
	UINT16		VID;		/* 0001:0000 PCI Vendor ID */
	UINT16		SSVID;		/* 0003:0002 PCI Subsystem Vendor ID */
	char		SN[20];		/* 0023:0004 Serial Number */
	char		MN[40];		/* 0063:0024 Model Number */
	char		FR[8];		/* 0071:0064 Firmware Revision */
	UINT8		RAB;		/* 0072:0072 Recommnended Arbitration Burst */
	UINT8		IEEE[3];	/* 0075:0073 IEEE OUI Identifier */
	UINT8		CMIC;		/* 0076:0076 Controller Multi-path I/O and Namespace Sharing Capabilities */
	UINT8		MDTS;		/* 0077:0077 Maximum Data Transfer Size */
	UINT16		CNTID;		/* 0079:0078 Controller ID */
	UINT32		VER;		/* 0083:0080 Version */
	UINT32		RTD3R;		/* 0087:0084 RTD3 Resume Latency */
	UINT32		RTD3E;		/* 0091:0088 RTD3 Entry Latency */
	UINT32		OAES;		/* 0095:0092 Optional Asynchronous Events Supported */
	UINT32		CTRATT;		/* 0099:0096 Controller Attributes */
	UINT8		rsvd100[12];	/* 0111:0100 */
	UINT8		FGUID[16];	/* 0127:0112 FRU Globally Unique Identifier */
	UINT8		rsvd128[128];	/* 0255:0128 */
	UINT16		OACS;		/* 0257:0256 Optional Admin Command Support */
	UINT8		ACL;		/* 0258:0258 Abort Command Limit */
	UINT8		AERL;		/* 0259:0259 Asynchronous Event Request Limit */
	UINT8		FRMW;		/* 0260:0260 Firmware Updates */
	UINT8		LPA;		/* 0261:0261 Log Page Attributes */
	UINT8		ELPE;		/* 0262:0262 Error Log Page Entries */
	UINT8		NPSS;		/* 0263:0263 Number of Power States Support */
	UINT8		AVSCC;		/* 0264:0264 Admin Vendor Specific Command Configuration */
	UINT8		APSTA;		/* 0265:0265 Autonomous Power State Transition Attributes */
	UINT16		WCTEMP;		/* 0267:0266 Warning Composite Temperature Threshold */
	UINT16		CCTEMP;		/* 0269:0268 Critical Composite Temperature Threshold */
	UINT16		MTFA;		/* 0271:0270 Maximum Time for Firmware Activation */
	UINT32		HMPRE;		/* 0275:0272 Host Memory Buffer Preferred Size */
	UINT32		HMMIN;		/* 0279:0276 Host Memory Buffer Minimum Size */
	UINT64		TNVMCAP[2];	/* 0295:0280 Total NVM Capacity */
	UINT64		UNVMCAP[2];	/* 0311:0296 Unallocated NVM Capacity */
	UINT32		RPMBS;		/* 0315:0312 Replay Protected Memory Block Support */
	UINT16		EDSTT;		/* 0317:0316 Extended Device Self-test Time */
	UINT8		DSTO;		/* 0318:0318 Device Self-test Options */
	UINT8		FWUG;		/* 0319:0319 Firmware Update Granularity */
	UINT16		KAS;		/* 0321:0320 Keep Alive Support */
	UINT16		HCTMA;		/* 0323:0322 Host Controlled Thermal Management Attributes */
	UINT16		MNTMT;		/* 0325:0324 Minimum Thermal Management Temperature */
	UINT16		MXTMT;		/* 0327:0326 Maximum Thermal Management Temperature */
	UINT32		SANICAP;	/* 0331:0328 Sanitize Capabilities */
	UINT8		rsvd332[180];	/* 0511:0332 */
	UINT8		SQES;		/* 0512:0512 Submission Queue Entry Size */
	UINT8		CQES;		/* 0513:0513 Completion Queue Entry Size */
	UINT16		MAXCMD;		/* 0515:0514 Maximum Outstanding Commands */
	UINT32		NN;		/* 0519:0516 Number of Namespaces */
	UINT16		ONCS;		/* 0521:0520 Optional NVM Command Support */
	UINT16		FUSES;		/* 0523:0522 Fused Operation Support */
	UINT8		FNA;		/* 0524:0524 Format NVM Attributes */
	UINT8		VMC;		/* 0525:0525 Volatile Write Cache */
	UINT16		AWUN;		/* 0527:0526 Atomic Write Unit Normal */
	UINT16		AWUPF;		/* 0529:0528 Atomic Write Unit Power Fail */
	UINT8		NVSCC;		/* 0530:0530 NVM Vendor Specific Command Configuration */
	UINT8		rsvd531;	/* 0531:0531 */
	UINT16		ACWU;		/* 0533:0532 Atomic Compare & Write Unit */
	UINT16		rsvd534;	/* 0535:0534 */
	UINT32		SGLS;		/* 0539:0536 SGL Support */
	UINT8		rsvd540[228];	/* 0767:0540 */
	UINT8		SUBNQN[256];	/* 1023:0768 NVM Subsystem NVMe Qualified Name */
	UINT8		rsvd1024[1024];	/* 2047:1024 */
	NVME_PSD	PSD[32];	/* 3071:2048 Power State 0 Descriptor */
	UINT8		VS[1024];	/* 4095:3072 Vendor Specific */
} IdentifyControllerData;

CC_ASSERT(4096 == sizeof (IdentifyControllerData));

typedef struct {
	UINT64		NSZE;		/* 0007:0000 Namespace Size */
	UINT64		NCAP;		/* 0015:0008 Namespace Capacity */
	UINT64		NUSE;		/* 0023:0016 Namespace Utilization */
	UINT8		NSFEAT;		/* 0024:0024 Namespace Features */
	UINT8		NLBAF;		/* 0025:0025 Number of LBA Formats */
	UINT8		FLBAS;		/* 0026:0026 Formatted LBA Size */
	UINT8		MC;		/* 0027:0027 Metadata Capabilities */
	UINT8		DPC;		/* 0028:0028 End-to-end Data Protection Capabilities */
	UINT8		DPS;		/* 0029:0029 End-to-end Data Protection Type Settings */
	UINT8		NMIC;		/* 0030:0030 Namespace Multi-path I/O and Namespace Sharing Capabilities */
	UINT8		RESCAP;		/* 0031:0031 Reservation Capabilities */
	UINT8		FPI;		/* 0032:0032 Format Progress Indicator */
	UINT8		DLFEAT;		/* 0033:0033 Deallocate Logical Block Features */
	UINT16		NAWUN;		/* 0035:0034 Namespace Atomic Write Unit Normal */
	UINT16		NAWUPF;		/* 0037:0036 Namespace Atomic Write Unit Power Fail */
	UINT16		NACWU;		/* 0039:0038 Namespace Atomic Compare & Write Unit */
	UINT16		NABSN;		/* 0041:0040 Namespace Atomic Boundary Size Normal */
	UINT16		NABO;		/* 0043:0042 Namespace Atomic Boundary Offset */
	UINT16		NABSPF;		/* 0045:0044 Namespace Atomic Boundary Size Power Fail */
	UINT16		NOIOB;		/* 0047:0046 Namespace Optimal IO Boundary */
	UINT64		NVMCAP[2];	/* 0063:0048 NVM Capacity */
	UINT8		rsvd64[40];	/* 0103:0064 */
	UINT8		NGUID[16];	/* 0119:0104 Namespace Globally Unique Identifier */
	UINT64		EUI64;		/* 0127:0120 IEEE Extended Unique Identifier */
	UINT32		LBAF[16];	/* 0191:0128 LBA Format x Support */
	UINT8		rsvd192[192];	/* 0383:0192 */
	UINT8		VS[3712];	/* 4095:0384 Vendor Specific */
} IdentifyNamespaceData;

CC_ASSERT(4096 == sizeof (IdentifyNamespaceData));

#endif	/* _IDENTIFY_H */
