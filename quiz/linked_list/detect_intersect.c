#include <stdio.h>
#include "linked_list.h"

int isIntersect(struct node *head1, struct node *head2)
{
	if (NULL == head1 || NULL == head2)
		return 0;

	while (NULL != head1->next)
		head1 = head1->next;

	while (NULL != head2->next)
		head2 = head2->next;

	return head1 == head2;
}

int main(void)
{
	struct node *list1 = LinkedList_Create(8);
	struct node *list2 = LinkedList_Create(8);

	if (isIntersect(list1, list2)) {
		puts("2 linked listed intersect");
	} else {
		puts("no intersection");
	}

	LinkedList_Destroy(list2);
	LinkedList_Destroy(list1);

	return 0;
}

