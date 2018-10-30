#ifndef _TIMESTAMP_H
#define	_TIMESTAMP_H

#ifdef __cplusplus
extern "C" {
#endif

#define	INVALID_MILLISECONDS	(~0u)

typedef struct {
	UINT32 seconds;
	UINT32 milliseconds;
} Timestamp;

CC_STATIC_ALWAYS_INLINE
void TimestampMarkInvalid(Timestamp *timestamp)
{
	timestamp->milliseconds = INVALID_MILLISECONDS;
}

CC_STATIC_ALWAYS_INLINE
BOOLEAN TimestampIsValid(const Timestamp *timestamp)
{
	return (timestamp->milliseconds != INVALID_MILLISECONDS) ? TRUE : FALSE;
}

CC_STATIC_ALWAYS_INLINE
void TimestampCopy(Timestamp *target, const Timestamp *source)
{
	target->seconds = source->seconds;
	target->milliseconds = source->milliseconds;
}

CC_STATIC_ALWAYS_INLINE
UINT32 TimestampGetDeltaMilliseconds(const Timestamp *start, const Timestamp *end)
{
	UINT32 deltaSeconds = end->seconds - start->seconds;
	return deltaSeconds * 1000 + end->milliseconds - start->milliseconds;
}

#ifdef __cplusplus
}
#endif

#endif	/* _TIMESTAMP_H */
