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
    return getNode(list)->task;
}

struct list wrrs[3] = {};

void WRR_addTask(struct task *task, enum weight w)
{
	RR_addTask(&wrrs[w], task);
}

struct task *WRR_getTask(void)
{
	return RR_getTask(&wrrs[0]);
}

