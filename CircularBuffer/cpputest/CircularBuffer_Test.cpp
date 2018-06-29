#include "CppUTest/TestHarness.h"

extern "C" {
#include "../CircularBuffer.c"
}

TEST_GROUP(CircularBuffer)
{
	void setup() {
		buffer = new CircularBuffer;
	}

	void teardown() {
		delete buffer;
	}

	CircularBuffer	*buffer;
};

TEST(CircularBuffer, empty_after_creation)
{
	CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
}

TEST(CircularBuffer, not_empty_after_put)
{
	CircularBuffer_Put(buffer, 42);
	CHECK_FALSE(CircularBuffer_IsEmpty(buffer));
}

