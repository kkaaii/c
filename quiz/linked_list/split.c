#include <stdio.h>
#include "linked_list.h"

static void LinkedList_Insert(struct node **head, struct node *node)
{
	node->next = *head;
	*head = node;
}

void split(struct node **a, struct node **b, struct node **c)
{
	while (NULL != *a) {
		struct node *node = *a;
		*a = node->next;

		if (node->data & 1) {
			LinkedList_Insert(b, node);
		} else {
			LinkedList_Insert(c, node);
		}
	}
}

int main(void)
{
	struct node *list = LinkedList_Create(8);
	struct node *a = list;
	struct node *b = NULL;
	struct node *c = NULL;

	LinkedList_Print(a);
	split(&a, &b, &c);

	LinkedList_Print(b);
	LinkedList_Print(c);

	LinkedList_Destroy(list);

	return 0;
}

