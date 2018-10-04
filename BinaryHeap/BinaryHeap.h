#ifndef _BINARY_HEAP_H
#define	_BINARY_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int T;

typedef enum {
	eMaxHeap = 0,
	eMinHeap
} BinaryHeapType;

typedef struct BinaryHeap BinaryHeap;

BinaryHeap *BinaryHeap_Create(int capacity, BinaryHeapType type);
void BinaryHeap_Destroy(BinaryHeap *heap);

void BinaryHeap_Insert(BinaryHeap *heap, T data);
T BinaryHeap_Extract(BinaryHeap *heap);

#ifdef __cplusplus
}
#endif

#endif	/* _BINARY_HEAP_H */

