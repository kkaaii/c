#ifndef _SPLAY_TREE_H
#define _SPLAY_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

#define	PARENT(x)	tree[x].parent
#define	LCHILD(x)	tree[x].lchild
#define	RCHILD(x)	tree[x].rchild

struct SplayTreeNode {
	int parent;
	int lchild;
	int rchild;
};

extern struct SplayTreeNode tree[];

void SplayTree_Init(void);
void SplayTree_RotateRight(int x);
void SplayTree_RotateLeft(int x);
void SplayTree_Splay(int x);

#ifdef __cplusplus
}
#endif

#endif  /* _SPLAY_TREE_H */
