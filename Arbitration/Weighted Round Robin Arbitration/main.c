/******************************************************************************
** File Name: main.c
** Author:
** Creation Time: Wed Sep 20 23:05:15 2017
*/
#include <stdio.h>
#include "arbitration.h"

#define N   4

struct task {
	char	c;
};

int main(int argc, char *argv[])
{
	struct task tasksH[] = {'A', 'B', 'C', 'D'};
	struct task tasksM[] = {'a', 'b', 'c', 'd'};
	struct task tasksL[] = {'0', '1', '2', '3'};
    int i, n;

	if (argc < 2 || 1 != sscanf(argv[1], "%d", &n)) {
		n = N * 6;
	}

	WRR_setWeight(eWeightH, 3);
	WRR_setWeight(eWeightM, 2);
	WRR_setWeight(eWeightL, 1);

    for (i = 0; i < N; ++i) {
        WRR_addTask(&tasksM[i], eWeightM);
    }

    for (i = 0; i < n; ++i) {
        struct task *task = WRR_getTask();
		putchar(task->c);
    }
	putchar('\n');

    for (i = 0; i < N; ++i) {
        WRR_addTask(&tasksL[i], eWeightL);
    }

    for (i = 0; i < n; ++i) {
        struct task *task = WRR_getTask();
		putchar(task->c);
    }
	putchar('\n');

    for (i = 0; i < N; ++i) {
        WRR_addTask(&tasksH[i], eWeightH);
    }

    for (i = 0; i < n; ++i) {
        struct task *task = WRR_getTask();
		putchar(task->c);
    }
    putchar('\n');

    return 0;
}

