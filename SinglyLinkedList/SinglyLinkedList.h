#ifndef	_SINGLY_LINKED_LIST_H
#define	_SINGLY_LINKED_LIST_H

typedef struct SinglyLinkedNode SinglyLinkedNode;

struct	SinglyLinkedNode {
	SinglyLinkedNode *next;
};

typedef struct {
	SinglyLinkedNode *head;
	SinglyLinkedNode *tail;
} SinglyLinkedList;

void SinglyLinkedList_Init(SinglyLinkedList *list);

int SinglyLinkedList_IsEmpty(SinglyLinkedList *list);

void SinglyLinkedList_AddToHead(SinglyLinkedList *list, SinglyLinkedNode *node);
void SinglyLinkedList_AddToTail(SinglyLinkedList *list, SinglyLinkedNode *node);
SinglyLinkedNode *SinglyLinkedList_RemoveHead(SinglyLinkedList *list);

void SinglyLinkedList_Reverse(SinglyLinkedList *list);

#endif	/* _SINGLY_LINKED_LIST_H */

