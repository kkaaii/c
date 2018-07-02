#include <stdio.h>
#include "linked_list.h"

struct node *reverseByRecursion(struct node *head)
{
	if (NULL == head || NULL == head->next)
		return head;

	struct node *list = reverseByRecursion(head->next);
	head->next->next = head;
	head->next = NULL;

	return list;
}

int main(void)
{
	struct node *list = LinkedList_Create(8);
	struct node *head = list;

	LinkedList_Print(head);
	head = reverseByRecursion(head);
	LinkedList_Print(head);

	return 0;
}
	
