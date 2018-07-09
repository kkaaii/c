#include "CppUTest/TestHarness.h"

extern "C" {
#include "CircularlySinglyLinkedList.h"
}

TEST_GROUP(CircularlySinglyLinkedList)
{
	CircularlySinglyLinkedList	list;

	void setup() {
		CircularlySinglyLinkedList_Init(&list);
	}
};

TEST(CircularlySinglyLinkedList, EmptyAfterCreation)
{
	CHECK(CircularlySinglyLinkedList_IsEmpty(&list));
}

TEST(CircularlySinglyLinkedList, NotEmpty)
{
	CircularlySinglyLinkedNode node;

	CircularlySinglyLinkedList_Insert(&list, &node);
	CHECK(!CircularlySinglyLinkedList_IsEmpty(&list));
}

TEST(CircularlySinglyLinkedList, NotEmptyThenEmpty)
{
	CircularlySinglyLinkedNode node;

	CircularlySinglyLinkedList_Insert(&list, &node);
	CHECK(!CircularlySinglyLinkedList_IsEmpty(&list));
	POINTERS_EQUAL(&node, CircularlySinglyLinkedList_Remove(&list));
	CHECK(CircularlySinglyLinkedList_IsEmpty(&list));
}

TEST(CircularlySinglyLinkedList, InsertNodes)
{
	CircularlySinglyLinkedNode node[3];

	CircularlySinglyLinkedList_Insert(&list, &node[0]);
	CircularlySinglyLinkedList_Insert(&list, &node[1]);
	CircularlySinglyLinkedList_Insert(&list, &node[2]);

	POINTERS_EQUAL(&node[2], CircularlySinglyLinkedList_Remove(&list));
	POINTERS_EQUAL(&node[1], CircularlySinglyLinkedList_Remove(&list));
	POINTERS_EQUAL(&node[0], CircularlySinglyLinkedList_Remove(&list));

	CHECK(NULL == CircularlySinglyLinkedList_Remove(&list));
}

TEST(CircularlySinglyLinkedList, RemoveFromEmpty)
{
	CHECK(NULL == CircularlySinglyLinkedList_Remove(&list));
}

#if 0
TEST(CircularlySinglyLinkedList, Reverse)
{
	CircularlySinglyLinkedNode node[3];

	CircularlySinglyLinkedList_AddToHead(&list, &node[0]);
	CircularlySinglyLinkedList_AddToHead(&list, &node[1]);
	CircularlySinglyLinkedList_AddToHead(&list, &node[2]);

	CircularlySinglyLinkedList_Reverse(&list);

	POINTERS_EQUAL(&node[0], CircularlySinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[1], CircularlySinglyLinkedList_RemoveHead(&list));
	POINTERS_EQUAL(&node[2], CircularlySinglyLinkedList_RemoveHead(&list));
}
#endif

