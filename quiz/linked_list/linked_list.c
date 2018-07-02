#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct node *LinkedList_Create(size_t items)
{
	struct node *head = (struct node *)calloc(items, sizeof (struct node));
	struct node *node = head + items - 1;
	struct node *prev = node - 1;

	node->data = items;
	node->next = NULL;
	while (--items > 0) {
		prev->data = items;
		prev->next = node;

		node = prev;
		prev = node - 1;
	}

	return head;
}

void LinkedList_Destroy(struct node *self)
{
	free(self);
}

void LinkedList_Print(struct node *head)
{
	while (NULL != head) {
		printf("%p(%d) -> ", head, head->data);
		head = head->next;
	}
	putchar('\n');
}

