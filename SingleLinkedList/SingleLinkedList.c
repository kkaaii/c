#include "SingleLinkedList.h"

void SingleLinkedList_Init(SingleLinkedList *list)
{
	list->head = NULL;
	list->tail = NULL;
}

int SingleLinkedList_IsEmpty(SingleLinkedList *list)
{
	return NULL == list->head && NULL == list->tail;
}

void SingleLinkedList_AddToHead(
	SingleLinkedList	*list,
	SingleLinkedListNode	*node)
{
	node->next = list->head;
	list->head = node;

	if (NULL == list->tail) {
		list->tail = node;
	}
}

void SingleLinkedList_AddToTail(
	SingleLinkedList	*list,
	SingleLinkedListNode	*node)
{
	SingleLinkedListNode	*tail = list->tail;

	list->tail = node;
	node->next = NULL;

	if (NULL == tail) {
		list->head = node;
	} else {
		tail->next = node;
	}
}

SingleLinkedListNode *SingleLinkedList_RemoveHead(SingleLinkedList *list)
{
	SingleLinkedListNode	*node = list->head;

	if (node == list->tail) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		list->head = node->next;
	}

	return node;
}

