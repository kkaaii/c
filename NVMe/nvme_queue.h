/******************************************************************************
** File Name: nvme_queue.h
** Author:
** Creation Time: Thu Jan 11 03:16:43 2018
*/
#ifndef	_NVME_QUEUE_H
#define	_NVME_QUEUE_H

typedef	struct {
	UINT16	head;
	UINT16	tail;
	UINT16	mask;
	UINT16	valid	: 1;
	UINT16	prio	: 2;
	UINT16	rsvd	: 13;
} NVME_QUEUE;

/*
** 4.1.1 Empty Queue
**
** The queue is Empty when the Head entry pointer equals the Tail entry
** pointer. Figure 8 defines the Empty Queue condition.
*/
#define	NVME_QUEUE_IS_EMPTY(sq)	\
	((sq)->head == (sq)->tail)

/*
** 4.1.2 Full Queue
**
** The queue is Full when the Head equals one more than the Tail. The number
** of entries in a queue when full is one less than the queue size. Figure 9
** defines the Full Queue condition.
**
** Note: Queue wrap conditions shall be taken into account when determining
** whether a queue is Full.
*/
#define	NVME_QUEUE_IS_FULL(q)	\
	((q)->head == ((q)->mask & ((q)->tail + 1)))

void	NvmeQ_Init(NVME_QUEUE *q, UINT8 bits);
BOOL	NvmeQ_UpdateHead(NVME_QUEUE *q, UINT16 head);
BOOL	NvmeQ_UpdateTail(NVME_QUEUE *q, UINT16 tail);

#endif	/* _NVME_QUEUE_H */

