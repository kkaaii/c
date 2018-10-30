#ifndef _COMMON_LOG_H
#define	_COMMON_LOG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CC_STATIC_ALWAYS_INLINE static inline __attribute__((always_inline))

#define CAST_PTR(type)                          (type)(intptr_t)

#define BYTES_TO_DWORDS(bytes)  ((bytes) / sizeof (UINT32))
#define DWORDS_TO_BYTES(dwords) ((dwords) * sizeof (UINT32))

typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;

typedef enum {FALSE = 0, TRUE = !FALSE} BOOLEAN;

#ifdef __cplusplus
}
#endif

#endif	/* _COMMON_LOG_H */
