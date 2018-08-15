#include "CppUTest/TestHarness.h"

#include <stdio.h>
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

TEST(SegmentTree, Query)
{
	int	expected[][6] = {
		{ 2},
		{ 7,  5},
		{ 8,  6,  1},
		{12, 10,  5,  4},
		{21, 19, 14, 13,  9},
		{24, 22, 17, 16, 12, 3},
	};

	for (size_t i = 0; i < 6; ++i) {
		for (size_t j = 0; j < i; ++j) {
			T actual = SegmentTree_Query(mTree, j, i);
			LONGS_EQUAL(expected[i][j], actual);
		}
	}
}

TEST(SegmentTree, Update)
{
	int data[] = {-2, -5, -1, -4, -9, -3};

	for (size_t i = 0; i < 6; ++i) {
		SegmentTree_Update(mTree, i, i, data[i]);
		LONGS_EQUAL(0, SegmentTree_Query(mTree, i, i));
	}
}

