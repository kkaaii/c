/******************************************************************************
** File Name: ATA_Command.h
** Author:
** Creation Time: Sun Nov 26 21:51:17 2017
*/
#ifndef _ATA_COMMAND_H
#define	_ATA_COMMAND_H

#define	ATA_COMMAND_92H_DOWNLOAD_MICROCODE	0x92
#define	ATA_COMMAND_93H_DOWNLOAD_MICROCODE_DMA	0x93
	#define	SUBCMD_03H_SEGMENTED	0x03
	#define	SUBCMD_07H_SINGLE	0x07
	#define	SUBCMD_0EH_DEFERRED	0x0E
	#define	SUBCMD_0FH_ACTIVATE	0x0F

typedef struct {
	unsigned char	command;
	unsigned char	feature;
	unsigned char	count;
	unsigned long	loLBA;
} ATA_Command;

void completeCommand(int status);
void putCommand(ATA_Command *pCommand);
ATA_Command *getCommand(void);

#endif	/* _ATA_COMMAND_H */

