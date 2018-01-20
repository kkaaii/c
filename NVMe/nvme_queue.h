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
	UINT16	size;
} NVME_QUEUE;

/*
** 4.1.1 Empty Queue
**
** The queue is Empty when the Head entry pointer equals the Tail entry
** pointer. Figure 8 defines the Empty Queue condition.
*/
#define	NVME_QUEUE_IS_EMPTY(q)	((q)->head == (q)->tail)

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
#define	NVME_QUEUE_IS_FULL(q)	((q)->head == ((q)->tail + 1) % (1 + (q)->size))

/*
** 4.1.3 Queue Size
**
** The Queue Size is indicated in a 16-bit 0's based field that indicates the
** number of slots in the queue. The minimum size for a queue is two slots.
** The maximum size for either an I/O Submission Queue or an I/O Completion
** Queue is defined as 64K slots, limited by the maximum queue size supported
** by the controller that is reported in the CAP.MQES field. The maximum size
** for the Admin Submission and Admin Completion Queue is defined as 4K slots.
** One slot in each queue is not available for use due to Head and Tail entry
** pointer definition.
*/
#define	NVME_MIN_QSIZE	2
#define	NVME_MAX_QSIZE	63

#define	NVME_QSIZE_IS_VALID(qsize)	\
	(NVME_MIN_QSIZE <= (qsize) && (qsize) <= NVME_MAX_QSIZE)

#define	NVME_QUEUE_INIT(q, qsize)	do {	\
	(q)->head = (q)->tail = 0;		\
	(q)->size = (qsize);			\
} while (0)

BOOL	NvmeQ_UpdateHead(NVME_QUEUE *q, UINT16 head);
BOOL	NvmeQ_UpdateTail(NVME_QUEUE *q, UINT16 tail);

#endif	/* _NVME_QUEUE_H */

