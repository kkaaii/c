#include "CircularlySinglyLinkedList.h"

void CircularlySinglyLinkedList_Init(CircularlySinglyLinkedList *list)
{
	CircularlySinglyLinkedNode *head = &list->head;
	head->next = head;
}

int CircularlySinglyLinkedList_IsEmpty(CircularlySinglyLinkedList *list)
{
	CircularlySinglyLinkedNode *head = &list->head;
	return head == head->next;
}

void CircularlySinglyLinkedList_Insert(CircularlySinglyLinkedList *list, CircularlySinglyLinkedNode *node)
{
	CircularlySinglyLinkedNode *head = &list->head;

	node->next = head->next;
	head->next = node;
}

CircularlySinglyLinkedNode *CircularlySinglyLinkedList_Remove(CircularlySinglyLinkedList *list)
{
	CircularlySinglyLinkedNode *head = &list->head;
	CircularlySinglyLinkedNode *node = head->next;

	if (node == head) {
		return NULL;
	}

	head->next = node->next;
	node->next = NULL;

	return node;
}

#if 0
void CircularlySinglyLinkedList_Reverse(CircularlySinglyLinkedList *list)
{
	CircularlySinglyLinkedNode *prev = NULL;
	CircularlySinglyLinkedNode *node = list->head;
	CircularlySinglyLinkedNode *next;

	while (NULL != node) {
		next = node->next;

		node->next = prev;
		prev = node;
		node = next;
	}

	list->tail = list->head;
	list->head = prev;
}
#endif
