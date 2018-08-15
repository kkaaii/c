#include "CppUTest/TestHarness.h"

#include "SegmentTree.h"

TEST_GROUP(SegmentTree)
{
	SegmentTree *mTree;

	void setup() {
		int data[] = {2, 5, 1, 4, 9, 3};
		mTree = SegmentTree_Create(data, sizeof data / sizeof data[0]);
	}

	void teardown() {
		SegmentTree_Delete(mTree);
	}
};

TEST(SegmentTree, Create)
{
	LONGS_EQUAL(2, SegmentTree_Query(mTree, 0, 0));
	LONGS_EQUAL(5, SegmentTree_Query(mTree, 1, 1));
	LONGS_EQUAL(1, SegmentTree_Query(mTree, 2, 2));
	LONGS_EQUAL(4, SegmentTree_Query(mTree, 3, 3));
	LONGS_EQUAL(9, SegmentTree_Query(mTree, 4, 4));
	LONGS_EQUAL(3, SegmentTree_Query(mTree, 5, 5));
}

