#include <stdio.h>
#include "nvme.h"

void *HostMain(void *context CC_ATTRIB_UNUSED)
{
	ENTER();
	Host_Init();
	LEAVE();
	return NULL;
}

