#include "CppUTest/TestHarness.h"

extern "C" {
#include "SingleLinkedList.h"
}

TEST_GROUP(SingleLinkedList)
{
	SingleLinkedList	list;

	void setup() {
		SingleLinkedList_Init(&list);
	}
};

TEST(SingleLinkedList, EmptyAfterCreation)
{
	CHECK(SingleLinkedList_IsEmpty(&list));
}

