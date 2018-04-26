#ifndef	_NVME_H

#define	CC_CONCAT(x, y)		x##y
#define	CC_CONCAT_EXPAND(x, y)	CC_CONCAT(x, y)
#define	CC_ASSERT(x)		typedef int CC_CONCAT_EXPAND(_cc_assert_, __COUNTER__)[1/!!(x)]

#define	CC_ATTRIB_UNUSED	__attribute__((unused))
#define	CC_ATTRIB_PACKED	__attribute__((packed))
#define	CC_ATTRIB_ALIGNED(x)	__attribute__((aligned(x)))

#define	CC_INLINE		inline
#define	CC_ALWAYS_INLINE	inline __attribute__ ((always_inline))

#define	CC_STATIC		static
#define	CC_STATIC_INLINE	static inline
#define	CC_STATIC_ALWAYS_INLINE	static inline __attribute__ ((always_inline))

typedef enum {
	FALSE	= 0,
	TRUE	= !FALSE
} BOOL;

typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef	unsigned int	UINT32;
typedef	unsigned long	UINT64;

CC_ASSERT(1 == sizeof(UINT8));
CC_ASSERT(2 == sizeof(UINT16));
CC_ASSERT(4 == sizeof(UINT32));
CC_ASSERT(8 == sizeof(UINT64));

#include "nvme_queue.h"
#include "nvme_cq_entry.h"
#include "nvme_sq_entry.h"

#endif	/* _NVME_H */

