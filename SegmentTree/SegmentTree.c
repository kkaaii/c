#include <stdio.h>
#include <stdlib.h>
#include "SegmentTree.h"

#define	LCHILD(n)	((n) * 2)
#define	RCHILD(n)	((n) * 2 + 1)

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

#define	RECURSIVE	0

#if RECURSIVE
struct SegmentTree {
	size_t	lower;
	size_t	upper;
	T	value;
	T	delta;
};

static void _PushDown(SegmentTree *tree, size_t root)
{
	SegmentTree	*node = &tree[root];

	if (0 != node->delta) {
		SegmentTree	*ltree = &tree[LCHILD(root)];
		SegmentTree	*rtree = &tree[RCHILD(root)];

		ltree->delta += node->delta;
		rtree->delta += node->delta;

		ltree->value += node->delta * (T)(ltree->upper - ltree->lower + 1);
		rtree->value += node->delta * (T)(rtree->upper - rtree->lower + 1);

		node->delta = 0;
	}
}

static void _Build(SegmentTree *tree, size_t root, T data[], size_t lower, size_t upper)
{
	SegmentTree	*node = &tree[root];

	node->lower = lower;
	node->upper = upper;
	node->delta = 0;

	if (lower == upper) {
		node->value = data[lower];
	} else {
		size_t	lchild = LCHILD(root);
		size_t	rchild = RCHILD(root);
		size_t mid = (lower + upper) / 2;

		_Build(tree, lchild, data, lower, mid);
		_Build(tree, rchild, data, mid + 1, upper);

		node->value = tree[lchild].value + tree[rchild].value;
	}
}

SegmentTree *SegmentTree_Create(T data[], size_t items)
{
	size_t	n = NextPowerOfTwo(items);
	SegmentTree *tree = malloc(sizeof (SegmentTree) * n * 2);

	_Build(tree, 1, data, 0, items - 1);

	return tree;
}

static T _Query(SegmentTree *tree, size_t root, size_t lower, size_t upper)
{
	T		answer = 0;
	SegmentTree	*node = &tree[root];

	if (lower <= node->lower && node->upper <= upper) {
		answer = node->value;
	} else {
		size_t	lchild = LCHILD(root);
		size_t	rchild = RCHILD(root);

		_PushDown(tree, root);

		if (lower <= tree[lchild].upper) {
			answer += _Query(tree, lchild, lower, upper);
		}

		if (upper >= tree[rchild].lower) {
			answer += _Query(tree, rchild, lower, upper);
		}
	}

	return answer;
}

T SegmentTree_Query(SegmentTree *tree, size_t lower, size_t upper)
{
	return _Query(tree, 1, lower, upper);
}

static void _Update(SegmentTree *tree, size_t root, size_t lower, size_t upper, T delta)
{
	SegmentTree	*node = &tree[root];

	if (lower <= node->lower && node->upper <= upper) {
		node->value += delta * (T)(node->upper - node->lower + 1);
		node->delta += delta;
	} else {
		size_t	lchild = LCHILD(root);
		size_t	rchild = RCHILD(root);

		_PushDown(tree, root);

		if (lower <= tree[lchild].upper) {
			_Update(tree, lchild, lower, upper, delta);
		}

		if (upper >= tree[rchild].lower) {
			_Update(tree, rchild, lower, upper, delta);
		}

		node->value = tree[lchild].value + tree[rchild].value;
	}
	
}

void SegmentTree_Update(SegmentTree *tree, size_t lower, size_t upper, T delta)
{
	_Update(tree, 1, lower, upper, delta);
}

#else

struct SegmentTree {
	size_t	n;
	T	*value;
	T	*delta;
};

void _Build(SegmentTree *tree, T data[], size_t items)
{
	T	*value = &tree->value[tree->n];
	size_t	i;

	while (items--) {
		*value++ = *data++;
	}

	for (i = tree->n - 1; i > 0; --i) {
		tree->value[i] = tree->value[LCHILD(i)] + tree->value[RCHILD(i)];
		tree->delta[i] = 0;
	}
}

SegmentTree *SegmentTree_Create(T data[], size_t items)
{
	size_t	n = NextPowerOfTwo(items + 2);
	size_t	size = sizeof (SegmentTree) + sizeof (T) * n * 4;
	SegmentTree *tree = malloc(size);

	tree->n = n;
	tree->value = (T *)(tree + 1);
	tree->delta = tree->value + n * 2;

	_Build(tree, data, items);

	return tree;
}

T SegmentTree_Query(SegmentTree *tree, size_t lower, size_t upper)
{
	size_t	i = tree->n + lower - 1;
	size_t	j = tree->n + upper + 1;
	T	answer = 0;
	T	lcount = 0;
	T	rcount = 0;
	T	x;

	for (x = 1; i ^ j ^ 1; i >>= 1, j >>= 1, x <<= 1) {
		if (tree->delta[i]) {
			answer += tree->delta[i] * lcount;
		}

		if (tree->delta[j]) {
			answer += tree->delta[j] * rcount;
		}

		if (~i & 1) {
			answer += tree->value[i ^ 1];
			lcount += x;
		}

		if (j & 1) {
			answer += tree->value[j ^ 1];
			rcount += x;
		}
	}

	for (; i; i >>= 1, j >>= 1) {
		answer += tree->delta[i] * lcount;
		answer += tree->delta[j] * rcount;
	}

	return answer;
}

void SegmentTree_Update(SegmentTree *tree, size_t lower, size_t upper, T delta)
{
	size_t	i = tree->n + lower - 1;
	size_t	j = tree->n + upper + 1;
	T	lcount = 0;
	T	rcount = 0;
	T	x;

	for (x = 1; i ^ j ^ 1; i >>= 1, j >>= 1, x <<= 1) {
		tree->value[i] += delta * lcount;
		tree->value[i] += delta * rcount;

		if (~i & 1) {
			tree->delta[i ^ 1] += delta;
			tree->value[i ^ 1] += delta * x;
			lcount += x;
		}

		if (j & 1) {
			tree->delta[j ^ 1] += delta;
			tree->value[j ^ 1] += delta * x;
			rcount += x;
		}
	}

	for (; i ; i >>= 1, j >>= 1) {
		tree->value[i] += delta * lcount;
		tree->value[j] += delta * rcount;
	}
}
#endif

void SegmentTree_Delete(SegmentTree *tree)
{
	free(tree);
}

