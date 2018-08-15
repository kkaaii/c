#ifndef	_SEGMENT_TREE_H
#define	_SEGMENT_TREE_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef	int T;
typedef struct SegmentTree SegmentTree;

SegmentTree *SegmentTree_Create(T data[], size_t items);
void SegmentTree_Delete(SegmentTree *tree);
T SegmentTree_Query(SegmentTree *tree, size_t lower, size_t upper);

#ifdef	__cplusplus
}
#endif

#endif	/* _SEGMENT_TREE_H */

