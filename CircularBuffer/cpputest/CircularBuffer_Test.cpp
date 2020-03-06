#include "CppUTest/TestHarness.h"

extern "C" {
#include "../CircularBuffer.c"
}

#define	BUF_SIZE	8

TEST_GROUP(CircularBuffer)
{
	void setup() {
		self = CircularBuffer_Create(BUF_SIZE);
	}

	void teardown() {
		CircularBuffer_Destroy(self);
	}

	CircularBuffer	*self;
};

TEST(CircularBuffer, empty_after_creation)
{
	CHECK_TRUE(CircularBuffer_IsEmpty(self));
	CHECK_FALSE(CircularBuffer_IsFull(self));
}

TEST(CircularBuffer, not_empty_after_put)
{
	CircularBuffer_Put(self, 42);
	CHECK_FALSE(CircularBuffer_IsEmpty(self));
}

TEST(CircularBuffer, empty_after_remove_the_last_item)
{
	CircularBuffer_Put(self, 42);
	CHECK_FALSE(CircularBuffer_IsEmpty(self));
	CircularBuffer_Get(self);
	CHECK_TRUE(CircularBuffer_IsEmpty(self));
}

TEST(CircularBuffer, put_until_full)
{
	for (int i = 0; i < BUF_SIZE; ++i) {
		CHECK_FALSE(CircularBuffer_IsFull(self));
		CircularBuffer_Put(self, 42);
	}
	CHECK_TRUE(CircularBuffer_IsFull(self));
}

TEST(CircularBuffer, not_full_after_remove_one_item)
{
	for (int i = 0; i < BUF_SIZE; ++i) {
		CircularBuffer_Put(self, 42);
	}
	CircularBuffer_Get(self);
	CHECK_FALSE(CircularBuffer_IsFull(self));
}
