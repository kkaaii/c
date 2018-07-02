#include <stdio.h>
#include "linked_list.h"

struct node *getLastK(struct node *head, int k)
{
	if (k < 0)
		return NULL;

	struct node *slow = head;
	struct node *fast = head;

	while (k > 0 && NULL != fast) {
		fast = fast->next;
		--k;
	}

	if (k > 0)
		return NULL;

	while (NULL != fast) {
		fast = fast->next;
		slow = slow->next;
	}

	return slow;
}

#define	N	8

int main(void)
{
	struct node *list = LinkedList_Create(N);
	int	i;

	LinkedList_Print(list);
	for (i = 0; i < 8; ++i) {
		struct node *node = getLastK(list, i + 1);
		if (NULL != node) {
			printf("last %uth: %d\n", i, node->data);
		}
	}

	LinkedList_Destroy(list);

	return 0;
}

