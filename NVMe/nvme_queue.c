/******************************************************************************
** File Name: nvme_queue.c
** Author:
** Creation Time: Thu Jan 11 03:36:41 2018
*/
#include <stdio.h>
#include "nvme.h"
#include "nvme_queue.h"

void	NvmeQ_Init(NVME_QUEUE *q, UINT8 bits)
{
	q->head = 0;
	q->tail = 0;
	q->mask = (UINT16)((1 << bits) - 1);
}

CC_STATIC_ALWAYS_INLINE
BOOL	IsHeadOverTail(NVME_QUEUE *q, UINT16 head)
{
	if (q->head < q->tail) {
		if (!(q->head < head && head <= q->tail)) {
			return TRUE;
		}
	} else {
		if (q->tail < head && head <= q->head) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL	NvmeQ_UpdateHead(NVME_QUEUE *q, UINT16 head)
{
	if (NVME_QUEUE_IS_EMPTY(q)) {
		return FALSE;
	}

	if (head > q->mask) {
		return FALSE;
	}

	if (head == q->head) {
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
		if (!(q->tail < tail && tail < q->head)) {
			return TRUE;
		}
	} else {
		if (q->head <= tail && tail < q->tail) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL	NvmeQ_UpdateTail(NVME_QUEUE *q, UINT16 tail)
{
	if (NVME_QUEUE_IS_FULL(q)) {
		return FALSE;
	}

	if (tail > q->mask) {
		return FALSE;
	}

	if (tail == q->tail) {
		return FALSE;
	}

	if (IsTailOverHead(q, tail)) {
		return FALSE;
	}

	q->tail = tail;
	return TRUE;
}


