#ifndef	_SINGLY_LINKED_LIST_H
#define	_SINGLY_LINKED_LIST_H

typedef struct SinglyLinkedListNode SinglyLinkedListNode;

struct	SinglyLinkedListNode {
	SinglyLinkedListNode	*next;
};

typedef struct {
	SinglyLinkedListNode	*head;
	SinglyLinkedListNode	*tail;
} SinglyLinkedList;

void SinglyLinkedList_Init(SinglyLinkedList *list);

int SinglyLinkedList_IsEmpty(SinglyLinkedList *list);

void SinglyLinkedList_AddToHead(SinglyLinkedList *list, SinglyLinkedListNode *node);
void SinglyLinkedList_AddToTail(SinglyLinkedList *list, SinglyLinkedListNode *node);
SinglyLinkedListNode *SinglyLinkedList_RemoveHead(SinglyLinkedList *list);

#endif	/* _SINGLY_LINKED_LIST_H */

