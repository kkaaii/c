#ifndef _LOCAL_LOG_CTXT_H
#define	_LOCAL_LOG_CTXT_H

#include "Timestamp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	UINT32	*start;
	UINT32	*end;
	UINT32	*head;
        UINT32  *tail;

        Timestamp oldestTimestamp;
        Timestamp newestTimestamp;
} LocalLogCtxt;

CC_STATIC_ALWAYS_INLINE
void LocalLogCtxt_Initialize(LocalLogCtxt *logCtxt, UINT32 *start, UINT32 numDwords)
{
        logCtxt->start = start;
        logCtxt->end = logCtxt->start + numDwords;
        logCtxt->head = start;
        logCtxt->tail = start;
        
        TimestampMarkInvalid(&logCtxt->oldestTimestamp);
        TimestampMarkInvalid(&logCtxt->newestTimestamp);
}

CC_STATIC_ALWAYS_INLINE
UINT32 LocalLogCtxt_GetRemainingDwords(LocalLogCtxt *logCtxt)
{
        if (logCtxt->head > logCtxt->tail)
                return (UINT32)(logCtxt->head - logCtxt->tail);
        
        return (UINT32)(logCtxt->end - logCtxt->tail) + (UINT32)(logCtxt->head - logCtxt->start);
}

BOOLEAN LocalLogCtxt_WriteEntryHeader(LocalLogCtxt *logCtxt, const Timestamp *now, const char *message, UINT32 numExtraDwords);
void LocalLogCtxt_WriteDword(LocalLogCtxt *logCtxt, UINT32 dword);

#ifdef __cplusplus
}
#endif

#endif	/* _LOCAL_LOG_CTXT_H */
