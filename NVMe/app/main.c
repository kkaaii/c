#include <stdio.h>
#include <pthread.h>
#include "nvme/nvme.h"

extern void *HostMain(void *context);
extern void *DeviceMain(void *context);

#define	MODULE_NAME	""

int main(void)
{
	int		rc;
	pthread_t	hostThread;
	pthread_t	deviceThread;

	ENTER();
	rc = pthread_create(&hostThread, NULL, HostMain, NULL);
	if (rc) {
		LEAVE();
		return -1;
	}

	rc = pthread_create(&deviceThread, NULL, DeviceMain, NULL);
	if (rc) {
		LEAVE();
		return -1;
	}

	pthread_join(hostThread, NULL);
	pthread_cancel(deviceThread);
	pthread_join(deviceThread, NULL);

	LEAVE();
	return 0;
}

