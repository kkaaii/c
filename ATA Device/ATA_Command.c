/******************************************************************************
** File Name: ATA_Command.c
** Author:
** Creation Time: Mon Nov 27 00:09:09 2017
*/
#include <stdio.h>
#include "ATA_Command.h"

static ATA_Command	*gATA_Command;

void putCommand(ATA_Command *pCommand)
{
	if (NULL == gATA_Command)
		gATA_Command = pCommand;
}

ATA_Command *getCommand(void)
{
	return gATA_Command;
}

void completeCommand(int status)
{
	gATA_Command = NULL;
}

