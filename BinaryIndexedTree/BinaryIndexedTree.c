#include <stdlib.h>
#include "BinaryIndexedTree.h"

struct BinaryIndexedTree {
	size_t	n;
	T	*data;
};

BinaryIndexedTree *BinaryIndexedTree_Create(size_t n)
{
	size_t size = sizeof(BinaryIndexedTree) + sizeof(T) * (1 + n);
	BinaryIndexedTree *tree = calloc(size, 1);

	tree->n = n;
	tree->data = (T *)(tree + 1);

	return tree;
}

void BinaryIndexedTree_Delete(BinaryIndexedTree *tree)
{
	free(tree);
}

T BinaryIndexedTree_Query(BinaryIndexedTree *tree, size_t i)
{
	T	sum = 0;

	while (i > 0) {
		sum += tree->data[i];
		i &= i - 1;
	}

	return sum;
}

void BinaryIndexedTree_Update(BinaryIndexedTree *tree, size_t i, T delta)
{
	while (i <= tree->n) {
		tree->data[i] += delta;
		i += -i & i;
	}
}


/*
static T _GetAt(BinaryIndexedTree *tree, size_t i)
{
	
}

static void _PutAt(BinaryIndexedTree *tree, size_t i, T x)
{
}

void BinaryIndexedTree_PutAt(BinaryIndexedTree *tree, size_t i, T x)
{
	_PutAt(tree, i, x);
}

T BinaryIndexedTree_GetAt(BinaryIndexedTree *tree, size_t i)
{
	return _GetAt(tree, i);
}

*/
