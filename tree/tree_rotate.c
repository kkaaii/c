#include <string.h>
#include "tree_rotate.h"

struct node tree[20];

void tree_init(void)
{
	memset(tree, 0, sizeof tree);
}

void rotate_right(int x)
{
	int p = PARENT(x);
	int y = LCHILD(x);

	if (y) {
		LCHILD(x) = RCHILD(y);
		if (RCHILD(y)) {
			PARENT(RCHILD(y)) = x;
		}
		PARENT(y) = p;
		RCHILD(y) = x;
	}

	if (p) {
		if (x == LCHILD(p)) {
			LCHILD(p) = y;
		} else {
			RCHILD(p) = y;
		}
	}

	PARENT(x) = y;
}

void rotate_left(int x)
{
	int p = PARENT(x);
	int y = RCHILD(x);

	if (y) {
		RCHILD(x) = LCHILD(y);
		if (LCHILD(y)) {
			PARENT(LCHILD(y)) = x;
		}
		PARENT(y) = p;
		LCHILD(y) = x;
	}
	
	if (p) {
		if (x == LCHILD(p)) {
			LCHILD(p) = y;
		} else {
			RCHILD(p) = y;
		}
	}

	PARENT(x) = y;
}