#include "CommonLog.h"
#include "LocalLogCtxt.h"

#define	STRING_ID(string)				((UINT16)(CAST_PTR(UINT32)(string) >> 2))

#define	BITS_DELTA_MILLISECONDS		12
#define	MAX_DELTA_MILLISECONDS		(1u << BITS_DELTA_MILLISECONDS)
#define	MASK_DELTA_MILLISECONDS		(MAX_DELTA_MILLISECONDS - 1)

typedef union {
        UINT32  all;
        struct {
		UINT32  stringId		: 16;
		UINT32  deltaMilliseconds	: BITS_DELTA_MILLISECONDS;
                UINT32  numExtraDwords          : 3;
                UINT32  isTimestampAbsolute     : 1;
        } bits;
} EntryHeader;

CC_STATIC_ALWAYS_INLINE
UINT32 GetEntryDwords(const EntryHeader *header)
{
        return (UINT32)BYTES_TO_DWORDS(sizeof (*header)) +
                (UINT32)header->bits.isTimestampAbsolute +
                (UINT32)header->bits.numExtraDwords;
}

static void WriteDword(LocalLogCtxt *logCtxt, UINT32 dword);

void LocalLogCtxt_WriteDword(LocalLogCtxt *logCtxt, UINT32 dword)
{
        WriteDword(logCtxt, dword);
}

static void WriteDword(LocalLogCtxt *logCtxt, UINT32 dword)
{
	*logCtxt->tail = dword;
	
	if (logCtxt->tail + 1 >= logCtxt->end) {
		logCtxt->tail = logCtxt->start;
	} else {
		++logCtxt->tail;
	}
}

BOOLEAN LocalLogCtxt_WriteEntryHeader(LocalLogCtxt *logCtxt, const Timestamp *now, const char *message, UINT32 numExtraDwords)
{
	EntryHeader header;
	UINT32 deltaMilliseconds = MAX_DELTA_MILLISECONDS;

	if (TimestampIsValid(&logCtxt->newestTimestamp)) {
		deltaMilliseconds = TimestampGetDeltaMilliseconds(&logCtxt->newestTimestamp, now);
	}

	if (deltaMilliseconds < MAX_DELTA_MILLISECONDS) {
		header.bits.isTimestampAbsolute = FALSE;
		header.bits.deltaMilliseconds = deltaMilliseconds & MASK_DELTA_MILLISECONDS;
	} else {
		header.bits.isTimestampAbsolute = TRUE;
		header.bits.deltaMilliseconds = now->milliseconds & MASK_DELTA_MILLISECONDS;
	}

	TimestampCopy(&logCtxt->newestTimestamp, now);
	if (!TimestampIsValid(&logCtxt->oldestTimestamp)) {
		TimestampCopy(&logCtxt->oldestTimestamp, now);
	}
	
	header.bits.stringId = STRING_ID(message);
	header.bits.numExtraDwords = numExtraDwords & 7;
	
	if (GetEntryDwords(&header) >= LocalLogCtxt_GetRemainingDwords(logCtxt)) {
		return FALSE;
	}

	WriteDword(logCtxt, header.all);
	if (header.bits.isTimestampAbsolute) {
		WriteDword(logCtxt, now->seconds);
	}

	return TRUE;
}
