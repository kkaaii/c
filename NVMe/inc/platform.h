#ifndef	_PLATFORM_H
#define	_PLATFORM_H

#define	FAILED	"\033[31mFAILED\033[0m"
#define	PASSED	"\033[32mPASSED\033[0m"

void *malloc_align(UINT32 alignment, UINT32 size);

#endif	/* _PLATFORM_H */

