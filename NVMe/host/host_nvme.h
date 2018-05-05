#ifndef	_HOST_NVME_H
#define	_HOST_NVME_H

#define	HOST_PAGE_SIZE_BITS	12
#define	HOST_PAGE_SIZE		(1u << HOST_PAGE_SIZE_BITS)
#define	HOST_PAGE_SIZE_MASK	(HOST_PAGE_SIZE - 1)

#define	HOST_DBG_MSG(...)	DBG_MSG("[ host ]" __VA_ARGS__)

NVME_CID Host_IssueCommand(NVME_QUEUE *sq);

#endif	/* _HOST_NVME_H */

