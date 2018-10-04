#include "CppUTest/TestHarness.h"

#include "BinaryHeap.h"

TEST_GROUP(BinaryHeap)
{
	BinaryHeap	*mHeap;

	void setup() {
		mHeap = NULL;
	}

	void teardown() {
		if (NULL != mHeap) {
			BinaryHeap_Destroy(mHeap);
			mHeap = NULL;
		}
	}

	void buildHeap(BinaryHeapType type) {
		mHeap = BinaryHeap_Create(10, type);

		BinaryHeap_Insert(mHeap, 11);
		BinaryHeap_Insert(mHeap, 3);
		BinaryHeap_Insert(mHeap, 8);
		BinaryHeap_Insert(mHeap, 5);
		BinaryHeap_Insert(mHeap, 4);
	}
};

TEST(BinaryHeap, MaxHeap)
{
	buildHeap(eMaxHeap);

	LONGS_EQUAL(11, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(8, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(5, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(4, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(3, BinaryHeap_Extract(mHeap));
}

TEST(BinaryHeap, MinHeap)
{
	buildHeap(eMinHeap);

	LONGS_EQUAL(3, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(4, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(5, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(8, BinaryHeap_Extract(mHeap));
	LONGS_EQUAL(11, BinaryHeap_Extract(mHeap));
}

