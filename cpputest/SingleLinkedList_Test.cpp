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

TEST(SingleLinkedList, NotEmptyAfterAddingToHead)
{
	SingleLinkedListNode	node;

	SingleLinkedList_AddToHead(&list, &node);
	CHECK(!SingleLinkedList_IsEmpty(&list));
}

TEST(SingleLinkedList, NotEmptyAfterAddingToTail)
{
	SingleLinkedListNode	node;

	SingleLinkedList_AddToTail(&list, &node);
	CHECK(!SingleLinkedList_IsEmpty(&list));
}

TEST(SingleLinkedList, NotEmptyAfterAddingToHeadThenEmpty)
{
	SingleLinkedListNode	node;

	SingleLinkedList_AddToHead(&list, &node);
	CHECK(!SingleLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, SingleLinkedList_RemoveHead(&list));
	CHECK(SingleLinkedList_IsEmpty(&list));
}

