#include <stdio.h>
#include "linked_list.h"

struct node *reverse(struct node *head)
{
	if (NULL == head || NULL == head->next)
		return head;

	struct node	*prev = NULL;
	struct node	*next = NULL;
	while (NULL != head) {
		next = head->next;
		head->next = prev;
		prev = head;
		head = next;
	}

	return prev;
}

int main(void)
{
	struct node	*list = LinkedList_Create(8);
	struct node	*head = list;

	LinkedList_Print(head);
	head = reverse(head);
	LinkedList_Print(head);

	LinkedList_Destroy(list);

	return 0;
}

