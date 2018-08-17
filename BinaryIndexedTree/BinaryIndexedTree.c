#include <stdlib.h>
#include "BinaryIndexedTree.h"

/*
**                  .---------.----.- C[8]
**                  |         |    |    |
**        .----.- C[4]        |    |    |
**        |    |    |         |    |    |
**   .- C[2]   |    |    .- C[6]   |    |
**   |    |    |    |    |    |    |    |
** C[1]   |  C[3]   |  C[5]   |  C[7]   |
**   |    |    |    |    |    |    |    |
** A[1] A[2] A[3] A[4] A[5] A[6] A[7] A[8]
*/

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

