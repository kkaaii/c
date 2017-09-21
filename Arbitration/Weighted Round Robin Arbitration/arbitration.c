/******************************************************************************
** File Name: arbitration.c
** Author:
** Creation Time: Wed Sep 20 23:05:15 2017
*/
#include <stdio.h>
#include <stdlib.h>
#include "arbitration.h"

struct node {
    struct node *prev;
    struct node *next;
    struct task *task;
};

struct list {
	struct node *head;
	unsigned int wmax;
	unsigned int wcur;
};

static void addNode(struct list *list, struct node *node)
{
	struct node *head = list->head;
    struct node *prev;

    if (NULL == head) {
        list->head = node;
        node->prev = node;
    } else {
        prev = head->prev;
        node->prev = prev;
        prev->next = node;
        head->prev = node;
    }
    node->next = head;
}

static struct node *getNode(struct list *list)
{
    struct node *node = list->head;
	if (NULL != node) {
		list->head = node->next;
	}
    return node;
}

static void RR_addTask(struct list *list, struct task *task)
{
    struct node *node = malloc(sizeof (*node));
    node->task = task;
    addNode(list, node);
}

static struct task *RR_getTask(struct list *list)
{
	struct node *node = NULL;

	if (0 != list->wcur) {
		node = getNode(list);
	}

	return node ? node->task : NULL;
}

struct list wrrs[3] = {};
enum weight w = eWeightH;

void WRR_setWeight(enum weight w, unsigned int wmax)
{
	struct list *list = &wrrs[w];
	list->wmax = wmax;
	list->wcur = wmax;
}

void WRR_addTask(struct task *task, enum weight w)
{
	struct list *list = &wrrs[w];
	RR_addTask(list, task);
}

struct task *WRR_getTask(void)
{
	struct list *list = &wrrs[w];
	struct task *task = NULL;
	enum weight wcur = w;

	for (; w < eWeightMax; ++w, ++list) {
		task = RR_getTask(list);
		if (NULL != task) {
			--list->wcur;
			break;
		}
		list->wcur = list->wmax;
	}

	if (NULL == task) {
		list = &wrrs[0];
		for (w = 0; w <= wcur; ++w, ++list) {
			task = RR_getTask(list);
			if (NULL != task) {
				--list->wcur;
				break;
			}
			list->wcur = list->wmax;
		}
	}

	return task;
}

