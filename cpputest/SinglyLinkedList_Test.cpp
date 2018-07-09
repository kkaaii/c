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
	SinglyLinkedNode node;

	SinglyLinkedList_AddToHead(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToTail)
{
	SinglyLinkedNode node;

	SinglyLinkedList_AddToTail(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToHeadThenEmpty)
{
	SinglyLinkedNode node;

	SinglyLinkedList_AddToHead(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, SinglyLinkedList_RemoveHead(&list));
	CHECK(SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, NotEmptyAfterAddingToTailThenEmpty)
{
	SinglyLinkedNode node;

	SinglyLinkedList_AddToTail(&list, &node);
	CHECK(!SinglyLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, SinglyLinkedList_RemoveHead(&list));
	CHECK(SinglyLinkedList_IsEmpty(&list));
}

TEST(SinglyLinkedList, LastInFirstOut)
{
	SinglyLinkedNode node[3];

	SinglyLinkedList_AddToHead(&list, &node[0]);
	SinglyLinkedList_AddToHead(&list, &node[1]);
	SinglyLinkedList_AddToHead(&list, &node[2]);

	POINTERS_EQUAL(&node[2], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, FirstInFirstOut)
{
	SinglyLinkedNode node[3];

	SinglyLinkedList_AddToTail(&list, &node[0]);
	SinglyLinkedList_AddToTail(&list, &node[1]);
	SinglyLinkedList_AddToTail(&list, &node[2]);

	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[2], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, AddToHeadThenAddToTail)
{
	SinglyLinkedNode node[2];

	SinglyLinkedList_AddToHead(&list, &node[0]);
	SinglyLinkedList_AddToTail(&list, &node[1]);

	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
}

TEST(SinglyLinkedList, RemoveHeadThenAddToTail)
{
	SinglyLinkedNode node[3];

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

TEST(SinglyLinkedList, Reverse)
{
	SinglyLinkedNode node[3];

	SinglyLinkedList_AddToHead(&list, &node[0]);
	SinglyLinkedList_AddToHead(&list, &node[1]);
	SinglyLinkedList_AddToHead(&list, &node[2]);

	SinglyLinkedList_Reverse(&list);

	POINTERS_EQUAL(&node[0], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], SinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[2], SinglyLinkedList_RemoveHead(&list));
}

