/******************************************************************************
** File Name: nvme_queue.c
** Author:
** Creation Time: Thu Jan 11 03:36:41 2018
*/
#include <stdio.h>
#include "nvme.h"
#include "nvme_queue.h"

CC_STATIC_ALWAYS_INLINE
BOOL	IsHeadOverTail(NVME_QUEUE *q, UINT16 head)
{
	if (q->head <= q->tail) {
		/*
		**     H
		** x..xxv..vvx..x
		**          T
		*/
		if (!(q->head < head && head <= q->tail)) {
			return TRUE;
		}
	} else {
		/*
		**          H
		** v..vvx..xxv..v
		**     T
		*/
		if (q->tail < head && head <= q->head) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL	NvmeQ_UpdateHead(NVME_QUEUE *q, UINT16 head)
{
	/* covered by IsHeadOverTail */
	if (0 && NVME_QUEUE_IS_EMPTY(q)) {
		return FALSE;
	}

	if (head > q->size) {
		return FALSE;
	}

	/* covered by IsHeadOverTail */
	if (0 && head == q->head) {
		return FALSE;
	}

	if (IsHeadOverTail(q, head)) {
		return FALSE;
	}

	q->head = head;
	return TRUE;
}

CC_STATIC_ALWAYS_INLINE
BOOL	IsTailOverHead(NVME_QUEUE *q, UINT16 tail)
{
	if (q->tail < q->head) {
		/*
		**          H
		** x..xxv..vxx..x
		**     T
		*/
		if (!(q->tail < tail && tail < q->head)) {
			return TRUE;
		}
	} else {
		/*
		**     H
		** v..vxx..xxv..v
		**          T
		*/
		if (q->head <= tail && tail <= q->tail) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL	NvmeQ_UpdateTail(NVME_QUEUE *q, UINT16 tail)
{
	/* covered by IsTailOverHead */
	if (0 && NVME_QUEUE_IS_FULL(q)) {
		return FALSE;
	}

	if (tail > q->size) {
		return FALSE;
	}

	/* covered by IsTailOverHead */
	if (0 && tail == q->tail) {
		return FALSE;
	}

	if (IsTailOverHead(q, tail)) {
		return FALSE;
	}

	q->tail = tail;
	return TRUE;
}

