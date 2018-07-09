#include "CppUTest/TestHarness.h"

extern "C" {
#include "SinglyLinkedList.h"
}

TEST_GROUP(SinglyLinkedList)
{
	SinglyLinkedList	list;

	void setup() {
		SinglyLinkedList_Init(&list);
	}
};

TEST(SinglyLinkedList, EmptyAfterCreation)
{
	CHECK(SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToHead)
{
	SinglyLinkedListNode	node;

	SinglyLinkedList_AddToHead(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToTail)
{
	SinglyLinkedListNode	node;

	SinglyLinkedList_AddToTail(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToHeadThenEmpty)
{
	SinglyLinkedListNode	node;

	SinglyLinkedList_AddToHead(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, SinglyLinkedList_RemoveHead(&list));
	CHECK(SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToTailThenEmpty)
{
	SinglyLinkedListNode	node;

	SinglyLinkedList_AddToTail(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, SinglyLinkedList_RemoveHead(&list));
	CHECK(SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, LastInFirstOut)
{
	SinglyLinkedListNode	node[3];

	SinglyLinkedList_AddToHead(&list, &node[0]);
	SinglyLinkedList_AddToHead(&list, &node[1]);
	SinglyLinkedList_AddToHead(&list, &node[2]);

	POINTERS_EQUAL(&node[2], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, FirstInFirstOut)
{
	SinglyLinkedListNode	node[3];

	SinglyLinkedList_AddToTail(&list, &node[0]);
	SinglyLinkedList_AddToTail(&list, &node[1]);
	SinglyLinkedList_AddToTail(&list, &node[2]);

	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[2], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, AddToHeadThenAddToTail)
{
	SinglyLinkedListNode	node[2];

	SinglyLinkedList_AddToHead(&list, &node[0]);
	SinglyLinkedList_AddToTail(&list, &node[1]);

	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, RemoveHeadThenAddToTail)
{
	SinglyLinkedListNode	node[3];

	SinglyLinkedList_AddToTail(&list, &node[0]);
	SinglyLinkedList_AddToTail(&list, &node[1]);
	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
	SinglyLinkedList_AddToTail(&list, &node[2]);

	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[2], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, RemoveFromEmpty)
{
	CHECK(NULL == SinglyLinkedList_RemoveHead(&list));
}

