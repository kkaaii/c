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

TEST(SingleLinkedList, NotEmptyAfterAddingToTailThenEmpty)
{
	SingleLinkedListNode	node;

	SingleLinkedList_AddToTail(&list, &node);
	CHECK(!SingleLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, SingleLinkedList_RemoveHead(&list));
	CHECK(SingleLinkedList_IsEmpty(&list));
}

TEST(SingleLinkedList, LastInFirstOut)
{
	SingleLinkedListNode	node[3];

	SingleLinkedList_AddToHead(&list, &node[0]);
	SingleLinkedList_AddToHead(&list, &node[1]);
	SingleLinkedList_AddToHead(&list, &node[2]);

	POINTERS_EQUAL(&node[2], SingleLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SingleLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[0], SingleLinkedList_RemoveHead(&list));
}

TEST(SingleLinkedList, FirstInFirstOut)
{
	SingleLinkedListNode	node[3];

	SingleLinkedList_AddToTail(&list, &node[0]);
	SingleLinkedList_AddToTail(&list, &node[1]);
	SingleLinkedList_AddToTail(&list, &node[2]);

	POINTERS_EQUAL(&node[0], SingleLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SingleLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[2], SingleLinkedList_RemoveHead(&list));
}

