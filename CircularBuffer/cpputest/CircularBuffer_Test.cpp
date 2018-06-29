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

TEST(CircularBuffer, empty_after_remove_the_last_item)
{
	CircularBuffer_Put(buffer, 42);
	CHECK_FALSE(CircularBuffer_IsEmpty(buffer));
	CircularBuffer_Get(buffer);
	CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
}

