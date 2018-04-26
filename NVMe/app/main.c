#include <stdio.h>
#include <pthread.h>
#include "nvme.h"

#define	WATCH(x)	printf("%s = 0x%lx\n", #x, (UINT64)x)

NVME_CONTROLLER	nvmeController = {0}, *gpController = &nvmeController;

void *HostThread(void *context)
{
	Host_Init();

	Host_RingDoorbell_SQT(0, 0);
	Host_RingDoorbell_SQT(1, 0);
	Host_RingDoorbell_SQT(2, 0);
	Host_RingDoorbell_SQT(3, 0);
	Host_RingDoorbell_SQT(4, 0);
	Host_RingDoorbell_SQT(5, 0);
	Host_RingDoorbell_SQT(6, 0);
	Host_RingDoorbell_SQT(7, 0);
}

int main(void)
{
#if 1
	HostThread(NULL);
#else
	int		rc;
	pthread_t	hostThread;

	rc = pthread_create(&hostThread, NULL, HostThread, NULL);
#endif
	return 0;
}

