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
	list->head = node;
}

void SingleLinkedList_AddToTail(
	SingleLinkedList	*list,
	SingleLinkedListNode	*node)
{
	list->tail = node;
}

