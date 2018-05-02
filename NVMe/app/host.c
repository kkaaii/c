#include <stdio.h>
#include "nvme.h"

void *HostMain(void *context CC_ATTRIB_UNUSED)
{
	UINT16	sqid = NVME_SQID_ADMIN;
	UINT16	cqid = NVME_CQID_ADMIN;

	ENTER();
	Host_Init();

	for (;;) {
		sleep(1);

		Host_IssueCommand(sqid);
		if (0 == (rand() & 3))
			Host_RingDoorbell_SQT(sqid);

		Host_CheckResponse(cqid);
		if (0 == (rand() & 3))
			Host_RingDoorbell_CQH(cqid);
	}

	LEAVE();
	return NULL;
}

