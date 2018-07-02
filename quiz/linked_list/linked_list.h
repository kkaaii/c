#ifndef	_LINKED_LIST_H
#define	_LINKED_LIST_H

struct node {
	struct node	*next;
	int		data;
};

struct node *LinkedList_Create(size_t items);
void LinkedList_Destroy(struct node *head);
void LinkedList_Print(struct node *head);

#endif	/* _LINKED_LIST_H */

