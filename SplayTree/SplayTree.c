#include <string.h>
#include "SplayTree.h"

#define	N	15

struct SplayTreeNode tree[1 + N];

void SplayTree_Init(void)
{
	memset(tree, 0, sizeof tree);
}

void SplayTree_RotateRight(int x)
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

void SplayTree_RotateLeft(int x)
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

void SplayTree_Splay(int x)
{
	int p;	/* parent */
	int g;	/* grand-parent */

	while (0 != (p = PARENT(x))) {
		g = PARENT(p);
		if (0 == g) {
			if (x == LCHILD(p)) {	/* Zig Step */
				SplayTree_RotateRight(p);
			} else {		/* Zag Step */
				SplayTree_RotateLeft(p);
			}
		} else {
			if (x == LCHILD(p)) {
				if (p == LCHILD(g)) {	/* Zig-Zig Step */
					SplayTree_RotateRight(g);
					SplayTree_RotateRight(p);
				} else {		/* Zag-Zig Step */
					SplayTree_RotateRight(p);
					SplayTree_RotateLeft(g);
				}
			} else {
				if (p == LCHILD(g)) {	/* Zig-Zag Step */
					SplayTree_RotateLeft(p);
					SplayTree_RotateRight(g);
				} else {		/* Zag-Zag Step */
					SplayTree_RotateLeft(g);
					SplayTree_RotateLeft(p);
				}
			}
		}
	}
}