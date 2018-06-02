#include "nvme/nvme.h"
#include "nvme_device.h"

CC_STATIC	UINT32	gNumberOfQueues = MAX_QUEUES;

struct node {
	struct node	*prev;
	struct node	*next;
	NVME_QID	sqid;
	NVME_QID	cqid;
};

struct node nodes[MAX_QUEUES];

struct node *node_append(struct node *head, struct node *node)
{
	if (NULL == head) {
		head = node;
		node->prev = head;
	} else {
		struct node *prev = head->prev;
		head->prev = node;
		node->prev = prev;
		prev->next = node;
	}
	node->next = head;
	return head;
}

struct node *node_remove(struct node *head, struct node *node)
{
	struct node *prev = node->prev;
	struct node *next = node->next;

	if (prev == node) {
		return NULL;
	}

	prev->next = next;
	next->prev = prev;
	node->prev = NULL;
	node->next = NULL;

	return (head == node) ? next : head;
}

static struct {
	struct node	*node;
	UINT16		count;
} rr = {.node = NULL};

void rr_switch(void)
{
#if 1
	rr.node = rr.node->next;
	rr.count = 4;
#else
	UINT32 value = Device_GetFeatureValue(eFID_Arbitration, eSEL_Current);

	rr.node = rr.node->next;
	rr.count = (1 << ((FEAT_ARB *)&value)->AB);
#endif
}

struct node *rr_get(void)
{
	struct node *node = rr.node;
	if (0 == --rr.count) {
		rr_switch();
	}

	return node;
}

BOOL DeviceArbitration_AddQueuePair(NVME_QID sqid, NVME_QID cqid)
{
	DEV_DBG_MSG("SQID = %04Xh; CQID = %04Xh\n", sqid, cqid);

	if (sqid > gNumberOfQueues) {
		return FALSE;
	}
	ASSERT(sqid < MAX_QUEUES);

WATCH(nodes[0].prev);
WATCH(nodes[1].prev);
WATCH(nodes[2].prev);
WATCH(nodes[3].prev);
	struct node *node = &nodes[sqid];
	if (NULL != node->prev) {
		return FALSE;
	}

	node->sqid = sqid;
	node->cqid = cqid;
	rr.node = node_append(rr.node, node);
	return TRUE;
}

NVME_QID DeviceArbitration_DelQueuePair(NVME_QID sqid)
{
	NVME_QID	cqid;
	struct node	*node = rr.node;

	while (node->sqid != sqid) {
		node = node->next;
	}

	cqid = node->cqid;
	if (node == rr.node) {
		rr_switch();
	}
	rr.node = node_remove(rr.node, node);
	return cqid;
}

void DeviceArbitration_Init(void)
{
	memset(nodes, 0, sizeof nodes);
	DeviceArbitration_AddQueuePair(NVME_SQID_ADMIN, NVME_CQID_ADMIN);

	rr_switch();
}

void DeviceArbitration_GetQueuePair(NVME_QID *sqid, NVME_QID *cqid)
{
	struct node	*node = rr_get();

	*sqid = node->sqid;
	*cqid = node->cqid;
}

