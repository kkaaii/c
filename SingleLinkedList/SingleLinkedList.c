#include "SingleLinkedList.h"

void SingleLinkedList_Init(SingleLinkedList *list)
{
	list->head = NULL;
}

int SingleLinkedList_IsEmpty(SingleLinkedList *list)
{
	return NULL == list->head;
}

void SingleLinkedList_AddToHead(
	SingleLinkedList	*list,
	SingleLinkedListNode	*node)
{
	list->head = node;
}

