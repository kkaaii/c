#include "nvme/nvme.h"
#include "nvme_host.h"

#define	FAILED	"\033[31mFAILED\033[0m"
#define	PASSED	"\033[32mPASSED\033[0m"

BOOL HostTest_CheckStatus(NVME_STATUS expected, NVME_STATUS actual)
{
	if (expected != (actual & ~(eSF_DoNotRetry | eSF_More))) {
		HOST_MSG(FAILED": expected %04xh, actual %04xh\n", expected, actual);
		return FALSE;
	}

	HOST_MSG(PASSED": %04Xh\n", actual);
	return TRUE;
}

