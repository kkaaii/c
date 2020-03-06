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
	LONGS_EQUAL(0, CircularBuffer_GetCount(self));
}

TEST(CircularBuffer, not_empty_after_put)
{
	CircularBuffer_Enqueue(self, 42);
	LONGS_EQUAL(1, CircularBuffer_GetCount(self));
	CHECK_FALSE(CircularBuffer_IsEmpty(self));
}

TEST(CircularBuffer, empty_after_remove_the_last_item)
{
	CircularBuffer_Enqueue(self, 42);
	CHECK_FALSE(CircularBuffer_IsEmpty(self));
	CircularBuffer_Dequeue(self);
	CHECK_TRUE(CircularBuffer_IsEmpty(self));
}

TEST(CircularBuffer, put_until_full)
{
	for (int i = 0; i < BUF_SIZE; ++i) {
		CHECK_FALSE(CircularBuffer_IsFull(self));
		CircularBuffer_Enqueue(self, 42);
	}
	CHECK_TRUE(CircularBuffer_IsFull(self));
}

TEST(CircularBuffer, not_full_after_remove_one_item)
{
	for (int i = 0; i < BUF_SIZE; ++i) {
		CircularBuffer_Enqueue(self, 42);
	}
	CircularBuffer_Dequeue(self);
	CHECK_FALSE(CircularBuffer_IsFull(self));
}

TEST(CircularBuffer, enqueue_and_dequeue)
{
	int data = 42;

	CircularBuffer_Enqueue(self, data);
	LONGS_EQUAL(data, CircularBuffer_GetData(self));
}

TEST(CircularBuffer, stress)
{
	int count = 0;

	for (int i = 0; i < 10000; ++i) {
		if ((i * i) & 4) {
			if (!CircularBuffer_IsEmpty(self)) {
				CircularBuffer_Dequeue(self);
				--count;
			}
		} else {
			if (!CircularBuffer_IsFull(self)) {
				CircularBuffer_Enqueue(self, i);
				++count;
			}
		}
	}

	LONGS_EQUAL(count, CircularBuffer_GetCount(self));
}

