#include "CppUTest/TestHarness.h"

#include <string.h>
#include "CommonLog.h"
#include "LocalLogCtxt.h"

#define LOCAL_LOG_BUFFER_DWORDS 32

#define INC(p)  ((p) = ((p) == localLogCtxt.end - 1) ? localLogCtxt.start : (p) + 1)

TEST_GROUP(CommonLog_LocalLogCtxt)
{
        UINT32 localLogBuffer[LOCAL_LOG_BUFFER_DWORDS];
        LocalLogCtxt localLogCtxt;
        Timestamp currentTimestamp;
        
        void setup() {
                memset(localLogBuffer, 0, sizeof (localLogBuffer));
                LocalLogCtxt_Initialize(&localLogCtxt, localLogBuffer, LOCAL_LOG_BUFFER_DWORDS);
                
                currentTimestamp.seconds = 1;
                currentTimestamp.milliseconds = 2;
        }

        void writeLog(UINT32 dwords[], UINT32 numDwords) {
                CHECK_TRUE(LocalLogCtxt_WriteEntryHeader(&localLogCtxt, &currentTimestamp, NULL, numDwords));
                for (UINT32 i = 0; i < numDwords; ++i) {
                        LocalLogCtxt_WriteDword(&localLogCtxt, dwords[i]);
                }
        }

        void checkLog(UINT32 dwords[], UINT32 numDwords, UINT32 *tail, BOOLEAN isAbsoluteTimestamp) {
                if (isAbsoluteTimestamp) {
                        LONGS_EQUAL(0x80020000 | (numDwords << 28), *tail);  INC(tail);
                        LONGS_EQUAL(currentTimestamp.seconds, *tail); INC(tail);
                } else {
                        LONGS_EQUAL(0x0FA00000 | (numDwords << 28), *tail); INC(tail);
                }

                for (UINT32 i = 0; i < numDwords; ++i) {
                        LONGS_EQUAL(dwords[i], *tail); INC(tail);
                }
        }

        void writeNDwordsAfterInitialization(UINT32 numDwords) {
                UINT32 dwords[] = {3, 6, 2, 7};
                UINT32 *tail = localLogCtxt.tail;
                UINT32 remainingDwords = LocalLogCtxt_GetRemainingDwords(&localLogCtxt);

                writeLog(dwords, numDwords);
                checkLog(dwords, numDwords, tail, TRUE);
                LONGS_EQUAL(remainingDwords - (numDwords + 2), LocalLogCtxt_GetRemainingDwords(&localLogCtxt));
        }

        void writeNDwordsAfterShortInterval(UINT32 numDwords) {
                UINT32 dwords[] = {3, 6, 2, 7};
                UINT32 *tail = localLogCtxt.tail;
                UINT32 remainingDwords = LocalLogCtxt_GetRemainingDwords(&localLogCtxt);

                currentTimestamp.seconds += 4;
                writeLog(dwords, numDwords);
                checkLog(dwords, numDwords, tail, FALSE);
                LONGS_EQUAL(remainingDwords - (numDwords + 1), LocalLogCtxt_GetRemainingDwords(&localLogCtxt));
        }

        void writeNDwordsAfterLongInterval(UINT32 numDwords) {
                UINT32 dwords[] = {3, 6, 2, 7};
                UINT32 *tail = localLogCtxt.tail;
                UINT32 remainingDwords = LocalLogCtxt_GetRemainingDwords(&localLogCtxt);

                currentTimestamp.seconds += 5;
                writeLog(dwords, numDwords);
                checkLog(dwords, numDwords, tail, TRUE);
                LONGS_EQUAL(remainingDwords - (numDwords + 2), LocalLogCtxt_GetRemainingDwords(&localLogCtxt));
        }
};

TEST(CommonLog_LocalLogCtxt, GetRemainingDwords)
{
        UINT32 i;

        localLogCtxt.head = localLogCtxt.start;
        for (i = 0; i < LOCAL_LOG_BUFFER_DWORDS; ++i) {
                localLogCtxt.tail = localLogCtxt.start + i;
                LONGS_EQUAL(LOCAL_LOG_BUFFER_DWORDS - i, LocalLogCtxt_GetRemainingDwords(&localLogCtxt));
        }

        localLogCtxt.tail = localLogCtxt.start;
        for (i = 1; i < LOCAL_LOG_BUFFER_DWORDS; ++i) {
                localLogCtxt.head = localLogCtxt.start + i;
                LONGS_EQUAL(i, LocalLogCtxt_GetRemainingDwords(&localLogCtxt));
        }
}

TEST(CommonLog_LocalLogCtxt, LocalLogFull)
{
        for (UINT32 numDwords = 0; numDwords <= 4; ++numDwords) {
                LocalLogCtxt_Initialize(&localLogCtxt, localLogBuffer, numDwords + 2);
                CHECK_FALSE(LocalLogCtxt_WriteEntryHeader(&localLogCtxt, &currentTimestamp, NULL, numDwords));
                LONGS_EQUAL(numDwords + 2, LocalLogCtxt_GetRemainingDwords(&localLogCtxt));
        }
}


TEST(CommonLog_LocalLogCtxt, WriteZeroDwords)
{
        writeNDwordsAfterInitialization(0);
        writeNDwordsAfterShortInterval(0);
        writeNDwordsAfterLongInterval(0);
}

TEST(CommonLog_LocalLogCtxt, WriteOneDword)
{
        writeNDwordsAfterInitialization(1);
        writeNDwordsAfterShortInterval(1);
        writeNDwordsAfterLongInterval(1);
}

TEST(CommonLog_LocalLogCtxt, WriteTwoDwords)
{
        writeNDwordsAfterInitialization(2);
        writeNDwordsAfterShortInterval(2);
        writeNDwordsAfterLongInterval(2);
}

TEST(CommonLog_LocalLogCtxt, WriteThreeDwords)
{
        writeNDwordsAfterInitialization(3);
        writeNDwordsAfterShortInterval(3);
        writeNDwordsAfterLongInterval(3);
}

TEST(CommonLog_LocalLogCtxt, WriteFourDwords)
{
        writeNDwordsAfterInitialization(4);
        writeNDwordsAfterShortInterval(4);
        writeNDwordsAfterLongInterval(4);
}
 
