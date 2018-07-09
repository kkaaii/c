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

void SinglyLinkedList_AddToHead(SinglyLinkedList *list, SinglyLinkedNode *node)
{
	node->next = list->head;
	list->head = node;

	if (NULL == list->tail) {
		list->tail = node;
	}
}

void SinglyLinkedList_AddToTail(SinglyLinkedList *list, SinglyLinkedNode *node)
{
	SinglyLinkedNode *tail = list->tail;

	list->tail = node;
	node->next = NULL;

	if (NULL == tail) {
		list->head = node;
	} else {
		tail->next = node;
	}
}

SinglyLinkedNode *SinglyLinkedList_RemoveHead(SinglyLinkedList *list)
{
	SinglyLinkedNode *node = list->head;

	if (node == list->tail) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		list->head = node->next;
	}

	return node;
}

void SinglyLinkedList_Reverse(SinglyLinkedList *list)
{
	SinglyLinkedNode *prev = NULL;
	SinglyLinkedNode *node = list->head;
	SinglyLinkedNode *next;

	while (NULL != node) {
		next = node->next;

		node->next = prev;
		prev = node;
		node = next;
	}

	list->tail = list->head;
	list->head = prev;
}

