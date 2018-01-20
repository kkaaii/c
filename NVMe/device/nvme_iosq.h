/******************************************************************************
** File Name: nvme_iosq.h
** Author:
** Creation Time: Thu Jan 11 07:50:47 2018
*/
#ifndef	_NVME_IOSQ_H
#define	_NVME_IOSQ_H

#include "nvme_queue.h"

#define	NVME_SQ_IS_EMPTY(sq)	NVME_QUEUE_IS_EMPTY(&(sq)->q)
#define	NVME_SQ_IS_FULL(sq)	NVME_QUEUE_IS_FULL(&(sq)->q)

#define	NVME_SQID_ADMIN		0

typedef	struct {
	NVME_QUEUE	q;
	UINT16		cqid;

	UINT16		valid	: 1;
	UINT16		prio	: 2;
	UINT16		rsvd	: 13;
} NVME_SQ;

#endif	/* _NVME_IOSQ_H */

