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

static struct node *head = NULL;

static void addNode(struct node *node)
{
    struct node *prev;

    if (NULL == head) {
        head = node;
        node->prev = head;
    } else {
        prev = head->prev;
        node->prev = prev;
        prev->next = node;
        head->prev = node;
    }
    node->next = head;
}

static struct node *getNode(void)
{
    struct node *node = head;
    head = head->next;
    return node;
}

void RR_addTask(struct task *task)
{
    struct node *node = malloc(sizeof (*node));
    node->task = task;
    addNode(node);
}

struct task *RR_getTask(void)
{
    return getNode()->task;
}

