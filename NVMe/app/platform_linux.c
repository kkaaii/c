#include "nvme/nvme.h"

void *Platform_MemAlign(UINT32 alignment, UINT32 size)
{
	void *p;
	if (0 != posix_memalign(&p, alignment, size))
		return NULL;
	ASSERT(0 == (CAST_PTR(UINT64)(p) % alignment));
	return p;
}

