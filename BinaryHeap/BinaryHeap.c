#include <stdlib.h>
#include "BinaryHeap.h"

struct BinaryHeap {
	T   *buffer;
	int (*compare)(T d1, T d2);
	int capacity;
	int items;
};

static int MaxHeapify_Compare(T d1, T d2)
{
	return d1 < d2;
}

static int MinHeapify_Compare(T d1, T d2)
{
	return d1 > d2;
}

BinaryHeap *BinaryHeap_Create(int capacity, BinaryHeapType type)
{
	size_t size = sizeof (BinaryHeap) + (size_t)(1 + capacity) * sizeof(T);
	BinaryHeap *heap = malloc(size);

	heap->buffer = (T *)(heap + 1);
	heap->compare = (eMaxHeap == type) ? MaxHeapify_Compare: MinHeapify_Compare;
	heap->capacity = capacity;
	heap->items = 0;

	return heap;
}

void BinaryHeap_Destroy(BinaryHeap *heap)
{
	free(heap);
}

void BinaryHeap_Insert(BinaryHeap *heap, T data)
{
	T *buffer = heap->buffer;
	int child = ++heap->items;
	int parent;

	for (; 0 != (parent = child / 2); child = parent) {
		if (!(*heap->compare)(buffer[parent], data))
			break;
		buffer[child] = buffer[parent];
	}

	buffer[child] = data;
}

T BinaryHeap_Extract(BinaryHeap *heap)
{
	T *buffer = heap->buffer;
	T data = buffer[1];
	int parent;
	int child;

	for (parent = 1; (child = parent * 2) < heap->items; parent = child) {
		if (child + 1 < heap->items &&
			(*heap->compare)(buffer[child], buffer[child + 1]))
			++child;

		if (!(*heap->compare)(buffer[heap->items], buffer[child]))
			break;

		buffer[parent] = buffer[child];
	}
	buffer[parent] = buffer[heap->items];

	--heap->items;
	return data;
}

