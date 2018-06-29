#include "CppUTest/TestHarness.h"

extern "C" {
#include "../CircularBuffer.c"
}

TEST_GROUP(CircularBuffer)
{
	CircularBuffer	*buffer;
};

TEST(CircularBuffer, empty_after_creation)
{
	CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
}

