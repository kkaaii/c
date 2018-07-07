#ifndef	_SINGLE_LINKED_LIST_H
#define	_SINGLE_LINKED_LIST_H

typedef struct SingleLinkedListNode SingleLinkedListNode;

struct	SingleLinkedListNode {
	SingleLinkedListNode	*next;
};

typedef struct {
	SingleLinkedListNode	*head;
	SingleLinkedListNode	*tail;
} SingleLinkedList;

void SingleLinkedList_Init(SingleLinkedList *list);

int SingleLinkedList_IsEmpty(SingleLinkedList *list);

void SingleLinkedList_AddToHead(SingleLinkedList *list, SingleLinkedListNode *node);
void SingleLinkedList_AddToTail(SingleLinkedList *list, SingleLinkedListNode *node);
SingleLinkedListNode *SingleLinkedList_RemoveHead(SingleLinkedList *list);

#endif	/* _SINGLE_LINKED_LIST_H */

