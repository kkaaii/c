/******************************************************************************
** File Name: main.c
** Author:
** Creation Time: Wed Sep 20 23:05:15 2017
*/
#include <stdio.h>
#include "arbitration.h"

#define N   3

struct task {
    void (*run)(void);
};

static void A(void)
{
    putchar('A');
}

static void B(void)
{
    putchar('B');
}

static void C(void)
{
    putchar('C');
}

int main()
{
    struct task tasks[3] = {A, B, C}, *task;
    int i;

    for (i = 0; i < N; ++i) {
        WRR_addTask(&tasks[i], 0);
    }

    for (i = 0; i < N * 3; ++i) {
        task = WRR_getTask();
        (*task->run)();
    }

    putchar('\n');
    return 0;
}

