#include <stdio.h>
#include <stdlib.h>
#include "SegmentTree.h"

#define	LCHILD(n)	((n) * 2)
#define	RCHILD(n)	((n) * 2 + 1)

struct SegmentTree {
	size_t	n;
	T	*value;
};

static T sum(T x, T y)
{
	return x + y;
}

T (*op)(T i, T j) = sum;

static size_t NextPowerOfTwo(size_t x)
{
	--x;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;

	return ++x;
}

SegmentTree *SegmentTree_Create(T data[], size_t items)
{
	size_t n = NextPowerOfTwo(items + 2);
	SegmentTree *tree = malloc(sizeof (T) * n * 2 + sizeof(SegmentTree));
	T	*value = (T *)(tree + 1) + n;

	tree->n = n;
	tree->value = (T *)(tree + 1);
	while (items--) {
		*value++ = *data++;
	}

	while (--n) {
		tree->value[n] = op(tree->value[LCHILD(n)], tree->value[RCHILD(n)]);
	}

	return tree;
}

void SegmentTree_Delete(SegmentTree *tree)
{
	free(tree);
}

T SegmentTree_Query(SegmentTree *tree, size_t lower, size_t upper)
{
	T answer = 0;
	size_t i = tree->n + lower - 1;
	size_t j = tree->n + upper + 1;

	for (; i ^ j ^ 1; i >>= 1, j >>= 1) {
		if (~i & 1) answer = op(answer, tree->value[i ^ 1]);
		if ( j & 1) answer = op(answer, tree->value[j ^ 1]);
	}

	return answer;
}

