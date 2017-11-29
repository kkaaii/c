/******************************************************************************
** File Name: DownloadMicrocode.c
** Author:
** Creation Time: Sun Nov 26 21:07:06 2017
*/
#include <stdio.h>
#include "ATA_Vars.h"
#include "ATA_Command.h"
#include "DownloadMicrocode.h"

#ifdef	DEBUG
#define	DEBUG_MSG(s, ...)	printf(s "\n", ##__VA_ARGS__)
#else
#define	DEBUG_MSG(s, ...)
#endif

typedef enum {
	eDL0_PowerOn,
	eDL1_NoDlInProcess,
	eDL2_SubCmd07Download,
	eDL3_SubCmd03FirstSegment,
	eDL4_SubCmd03NextSegment,
	eDL5_SubCmd03SaveActivate,
	eDL6_SubCmd0eFirstSegment,
	eDL7_SubCmd0eNextSegment,
	eDL8_SubCmd0eSave,
	eDL9_SubCmd0fActivate
} EDlState;


static int DL0_PowerOn(void);
static int DL1_NoDlInProcess(void);
static int DL2_SubCmd07Download(void);
static int DL3_SubCmd03FirstSegment(void);
static int DL4_SubCmd03NextSegment(void);
static int DL5_SubCmd03SaveActivate(void);
static int DL6_SubCmd0eFirstSegment(void);
static int DL7_SubCmd0eNextSegment(void);
static int DL8_SubCmd0eSave(void);
static int DL9_SubCmd0fActivate(void);

static int (*states[])(void) = {
	DL0_PowerOn,
	DL1_NoDlInProcess,
	DL2_SubCmd07Download,
	DL3_SubCmd03FirstSegment,
	DL4_SubCmd03NextSegment,
	DL5_SubCmd03SaveActivate,
	DL6_SubCmd0eFirstSegment,
	DL7_SubCmd0eNextSegment,
	DL8_SubCmd0eSave,
	DL9_SubCmd0fActivate
};

static EDlState gState = eDL0_PowerOn;

static unsigned short	expectedBufferOffset;

static inline void changeState(EDlState state)
{
	DEBUG_MSG("Transition DL%d:DL%d", gState, state);
	gState = state;
}

static inline unsigned short getBlockCount(ATA_Command *pCommand)
{
	return ((pCommand->loLBA & 0xFF) << 8) | pCommand->count;
}

static inline unsigned short getBufferOffset(ATA_Command *pCommand)
{
	return (pCommand->loLBA >> 8) & 0xFFFF;
}

static int DL0_PowerOn(void)
{
	/* Tranistion DL0:DL1 */
	ATA_BitClr(bit_FW_ACTIVATION_PENDING);
	changeState(eDL1_NoDlInProcess);
	return 1;
}

static int DL1_NoDlInProcess(void)
{
	ATA_Command	*pCommand = getCommand();
	int		ret = 0;

	if (NULL == pCommand) {
		return ret;
	}

	switch (pCommand->command) {
	case ATA_COMMAND_92H_DOWNLOAD_MICROCODE:
	case ATA_COMMAND_93H_DOWNLOAD_MICROCODE_DMA:
		switch (pCommand->feature) {
		case SUBCMD_03H_SEGMENTED:
			/* Transition DL1:DL3 */
			if (0 == getBufferOffset(pCommand)) {
				changeState(eDL3_SubCmd03FirstSegment);
				ret = 1;
			}
			break;

		case SUBCMD_07H_SINGLE:
			/* Transition DL1:DL2 */
			changeState(eDL2_SubCmd07Download);
			ret = 1;
			break;

		case SUBCMD_0EH_DEFERRED:
			/* Transition DL1:DL6 */
			if (0 == getBufferOffset(pCommand)) {
				changeState(eDL6_SubCmd0eFirstSegment);
				ret = 1;
			}
			break;

		case SUBCMD_0FH_ACTIVATE:
			/* Transition DL1:DL9 */
			changeState(eDL9_SubCmd0fActivate);
			ret = 1;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return ret;
}

static int DL2_SubCmd07Download(void)
{
#ifdef	NOT_YET
	if (errorDetected()) {
		/* Transition DL2a:DL1 */
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
	} else if (resetDetected()) {
		/* Transition DL2b:DL1 */
		changeState(eDL1_NoDlInProcess);
#else
	if (0) {
#endif
	} else {
		/* Transition DL2c:DL1 */
		completeCommand(0);
		changeState(eDL1_NoDlInProcess);
	}

	return 1;
}

static int DL3_SubCmd03FirstSegment(void)
{
	ATA_Command	*pCommand = getCommand();
	unsigned short	blockCount;
	unsigned short	minimum = ATA_Field(DM_MINIMUM_TRANSFER_SIZE);
	unsigned short	maximum = ATA_Field(DM_MAXIMUM_TRANSFER_SIZE);

	blockCount = getBlockCount(pCommand);
	if ((0x0000 != minimum && 0xFFFF != minimum && blockCount < minimum) ||
	    (0x0000 != maximum && 0xFFFF != maximum && blockCount > maximum)) {
		/* Transition DL3a:DL1 */
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
#ifdef	NOT_YET
	} else if (resetDetected()) {
		/* Transition DL3b:DL1 */
		changeState(eDL1_NoDlInProcess);
	} else if (!dataComplete()) {
#else
	} else if (1) {
#endif
		/* Transition DL3:DL4 */
		expectedBufferOffset = getBufferOffset(pCommand) + blockCount;

		completeCommand(0);
		changeState(eDL4_SubCmd03NextSegment);
	} else {
		/* Transition DL3:DL5 */
		completeCommand(0);
		changeState(eDL5_SubCmd03SaveActivate);
	}

	return 1;
}

static int DL4_SubCmd03NextSegment(void)
{
	ATA_Command	*pCommand = getCommand();
	unsigned short	bufferOffset;
	unsigned short	blockCount;
	unsigned short	minimum = ATA_Field(DM_MINIMUM_TRANSFER_SIZE);
	unsigned short	maximum = ATA_Field(DM_MAXIMUM_TRANSFER_SIZE);

	if (NULL == pCommand) {
		return 0;
	}

	/* Transition DL4c:DL1 */
	if (pCommand->command != ATA_COMMAND_92H_DOWNLOAD_MICROCODE &&
	    pCommand->command != ATA_COMMAND_93H_DOWNLOAD_MICROCODE_DMA) {
		changeState(eDL1_NoDlInProcess);
		return 1;
	}

	switch (pCommand->feature) {
	case SUBCMD_07H_SINGLE:		/* Transition DL4:DL2 */
		changeState(eDL2_SubCmd07Download);
		return 1;

	case SUBCMD_03H_SEGMENTED:	/* Transition DL4:DL3 */
		if (0 == getBufferOffset(pCommand)) {
			changeState(eDL3_SubCmd03FirstSegment);
			return 1;
		}
		break;

	case SUBCMD_0EH_DEFERRED:	/* Tranistion DL4:DL6 */
		if (0 == getBufferOffset(pCommand)) {
			changeState(eDL6_SubCmd0eFirstSegment);
			return 1;
		}
		break;

	default:
		break;
	}

	blockCount = getBlockCount(pCommand);
	if (0x0000 == blockCount) {
		return 0;
	}

	bufferOffset = getBufferOffset(pCommand);
	if ((0x0000 != minimum && 0xFFFF != minimum && blockCount < minimum) ||
	    (0x0000 != maximum && 0xFFFF != maximum && blockCount > maximum) ||
	    (bufferOffset != expectedBufferOffset)) {
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
		return 1;
	}

	expectedBufferOffset = bufferOffset + blockCount;
	return 0;
}

static int DL5_SubCmd03SaveActivate(void)
{
	/* if () */ {
		completeCommand(0);
	}
	changeState(eDL1_NoDlInProcess);
	return 0;
}

static int DL6_SubCmd0eFirstSegment(void)
{
	ATA_Command	*pCommand = getCommand();
	unsigned short	blockCount;
	unsigned short	minimum = ATA_Field(DM_MINIMUM_TRANSFER_SIZE);
	unsigned short	maximum = ATA_Field(DM_MAXIMUM_TRANSFER_SIZE);

	blockCount = getBlockCount(pCommand);
	if ((0x0000 != minimum && 0xFFFF != minimum && blockCount < minimum) ||
	    (0x0000 != maximum && 0xFFFF != maximum && blockCount > maximum)) {
		/* Transition DL6a:DL1 */
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
#ifdef	NOT_YET
	} else if (resetDetected()) {
		/* Transition DL6b:DL1 */
		changeState(eDL1_NoDlInProcess);
	} else if (!dataComplete()) {
#else
	} else if (1) {
#endif
		/* Transition DL6:DL7 */
		expectedBufferOffset = getBufferOffset(pCommand) + blockCount;

		completeCommand(0);
		changeState(eDL7_SubCmd0eNextSegment);
	} else {
		/* Transition DL6:DL8 */
		completeCommand(0);
		changeState(eDL8_SubCmd0eSave);
	}

	return 1;
}

static int DL7_SubCmd0eNextSegment(void)
{
	ATA_Command	*pCommand = getCommand();
	unsigned short	bufferOffset;
	unsigned short	blockCount;
	unsigned short	minimum = ATA_Field(DM_MINIMUM_TRANSFER_SIZE);
	unsigned short	maximum = ATA_Field(DM_MAXIMUM_TRANSFER_SIZE);

	if (NULL == pCommand) {
		return 0;
	}

	/* Transition DL7c:DL1 */
	if (pCommand->command != ATA_COMMAND_92H_DOWNLOAD_MICROCODE &&
	    pCommand->command != ATA_COMMAND_93H_DOWNLOAD_MICROCODE_DMA) {
		changeState(eDL1_NoDlInProcess);
		return 1;
	}

	switch (pCommand->feature) {
	case SUBCMD_07H_SINGLE:		/* Transition DL7:DL2 */
		changeState(eDL2_SubCmd07Download);
		return 1;

	case SUBCMD_03H_SEGMENTED:	/* Transition DL7:DL3 */
		if (0 == getBufferOffset(pCommand)) {
			changeState(eDL3_SubCmd03FirstSegment);
			return 1;
		}
		break;

	case SUBCMD_0EH_DEFERRED:	/* Tranistion DL7:DL6 */
		if (0 == getBufferOffset(pCommand)) {
			changeState(eDL6_SubCmd0eFirstSegment);
			return 1;
		}
		break;

	default:
		break;
	}

	blockCount = getBlockCount(pCommand);
	if (0x0000 == blockCount) {
		return 0;
	}

	bufferOffset = getBufferOffset(pCommand);
	if ((0x0000 != minimum && 0xFFFF != minimum && blockCount < minimum) ||
	    (0x0000 != maximum && 0xFFFF != maximum && blockCount > maximum) ||
	    (bufferOffset != expectedBufferOffset)) {
		/* Transition DL7a:DL1 */
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
		return 1;
	}

#ifdef	NOT_YET
	if (resetDetected()) {
		/* Transition DL7b:DL1 */
		changeState(eDL1_NoDlInProcess);
		return 1;
	}

	if (allDownloaded()) {
		/* Transition DL7:DL8 */
		changeState(eDL8_SubCmd0eSave);
		return 1;
	}
#endif

	expectedBufferOffset = bufferOffset + blockCount;
	return 0;
}

static int DL8_SubCmd0eSave(void)
{
#ifdef	NOT_YET
	if (errorDetected()) {
		/* Transition DL8a:DL1 */
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
	} else if (resetDetected()) {
		/* Transition DL8b:DL1 */
		changeState(eDL1_NoDlInProcess);
#else
	if (0) {
#endif
	} else {
		/* Transition DL8c:DL1 */
		ATA_BitSet(bit_FW_ACTIVATION_PENDING);
		completeCommand(0);
		changeState(eDL1_NoDlInProcess);
	}

	return 1;
}

static int DL9_SubCmd0fActivate(void)
{
#ifdef	NOT_YET
	if (errorDetected()) {
#else
	if (0) {
#endif
		/* Transition DL9a:DL1 */
		ATA_BitClr(bit_FW_ACTIVATION_PENDING);
		completeCommand(0);
		changeState(eDL1_NoDlInProcess);
	} else {
		/* Transition DL9b:DL1 */
		ATA_BitClr(bit_FW_ACTIVATION_PENDING);
		completeCommand(-1);
		changeState(eDL1_NoDlInProcess);
	}
		
	return 0;
}

void ATA_Command_DownloadMicrocode(void)
{
	while ((*states[gState])()) {
	}
}

