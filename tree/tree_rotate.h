#ifndef _TREE_ROTATE_H
#define _TREE_ROTATE_H

#ifdef __cplusplus
extern "C" {
#endif

struct node {
	int parent;
	int lchild;
	int rchild;
};

extern struct node tree[];

#define	PARENT(x)	tree[x].parent
#define	LCHILD(x)	tree[x].lchild
#define	RCHILD(x)	tree[x].rchild

void tree_init(void);
void rotate_right(int x);
void rotate_left(int x);

#ifdef __cplusplus
}
#endif

#endif
