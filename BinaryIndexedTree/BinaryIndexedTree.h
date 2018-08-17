#ifndef	_BINARY_INDEXED_TREE_H
#define	_BINARY_INDEXED_TREE_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef int	T;

typedef struct BinaryIndexedTree BinaryIndexedTree;

BinaryIndexedTree *BinaryIndexedTree_Create(size_t n);
void BinaryIndexedTree_Delete(BinaryIndexedTree *tree);

T BinaryIndexedTree_Query(BinaryIndexedTree *tree, size_t i);

void BinaryIndexedTree_Update(BinaryIndexedTree *tree, size_t i, T delta);

#ifdef	__cplusplus
}
#endif

#endif	/* _BINARY_INDEXED_TREE_H */

