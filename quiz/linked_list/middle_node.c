#include <stdio.h>
#include "linked_list.h"

struct node *getMiddleNode(struct node *head)
{
	if (NULL == head)
		return NULL;

	struct node *slow = head;
	struct node *fast = head;

	while (NULL != fast && NULL != fast->next) {
		fast = fast->next->next;
		slow = slow->next;
	}

	return slow;
}

#define	N	8

int main(void)
{
	struct node *list = LinkedList_Create(N);
	struct node *node = getMiddleNode(list);

	LinkedList_Print(list);
	if (NULL != node) {
		printf("the middle node: %p(%d)\n", node, node->data);
	}

	LinkedList_Destroy(list);

	return 0;
}

