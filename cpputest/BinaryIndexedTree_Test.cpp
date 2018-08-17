#include "CppUTest/TestHarness.h"

#include "BinaryIndexedTree.h"

#define	N	6

TEST_GROUP(BinaryIndexedTree)
{
	BinaryIndexedTree *mTree;

	void setup() {
		mTree = BinaryIndexedTree_Create(N);
	}

	void teardown() {
		BinaryIndexedTree_Delete(mTree);
	}
};

TEST(BinaryIndexedTree, Create)
{
	for (size_t i = 1; i <= N; ++i) {
		LONGS_EQUAL(0, BinaryIndexedTree_Query(mTree, i));
	}
}

TEST(BinaryIndexedTree, Update)
{
	T data[N] = {2, 5, 1, 4, 9, 3};
	T expected[N] = {2, 7, 8, 12, 21, 24};

	for (size_t i = 0; i < N; ++i) {
		BinaryIndexedTree_Update(mTree, i + 1, data[i]);
		LONGS_EQUAL(expected[i], BinaryIndexedTree_Query(mTree, i + 1));
	}
}

