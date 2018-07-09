#ifndef	_CIRCULARLY_SINGLY_LINKED_LIST_H
#define	_CIRCULARLY_SINGLY_LINKED_LIST_H

typedef struct CircularlySinglyLinkedNode CircularlySinglyLinkedNode;

struct	CircularlySinglyLinkedNode {
	CircularlySinglyLinkedNode *next;
};

typedef struct {
	CircularlySinglyLinkedNode head;
} CircularlySinglyLinkedList;

void CircularlySinglyLinkedList_Init(CircularlySinglyLinkedList *list);

int CircularlySinglyLinkedList_IsEmpty(CircularlySinglyLinkedList *list);

void CircularlySinglyLinkedList_Insert(CircularlySinglyLinkedList *list, CircularlySinglyLinkedNode *node);
CircularlySinglyLinkedNode *CircularlySinglyLinkedList_Remove(CircularlySinglyLinkedList *list);

void CircularlySinglyLinkedList_Reverse(CircularlySinglyLinkedList *list);

#endif	/* _CIRCULARLY_SINGLY_LINKED_LIST_H */

