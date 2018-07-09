#include "SinglyLinkedList.h"

void SinglyLinkedList_Init(SinglyLinkedList *list)
{
	list->head = NULL;
	list->tail = NULL;
}

int SinglyLinkedList_IsEmpty(SinglyLinkedList *list)
{
	return NULL == list->head && NULL == list->tail;
}

void SinglyLinkedList_AddToHead(
	SinglyLinkedList	*list,
	SinglyLinkedListNode	*node)
{
	node->next = list->head;
	list->head = node;

	if (NULL == list->tail) {
		list->tail = node;
	}
}

void SinglyLinkedList_AddToTail(
	SinglyLinkedList	*list,
	SinglyLinkedListNode	*node)
{
	SinglyLinkedListNode	*tail = list->tail;

	list->tail = node;
	node->next = NULL;

	if (NULL == tail) {
		list->head = node;
	} else {
		tail->next = node;
	}
}

SinglyLinkedListNode *SinglyLinkedList_RemoveHead(SinglyLinkedList *list)
{
	SinglyLinkedListNode	*node = list->head;

	if (node == list->tail) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		list->head = node->next;
	}

	return node;
}

