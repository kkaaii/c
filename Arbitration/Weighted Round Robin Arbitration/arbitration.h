/******************************************************************************
** File Name: arbitration.h
** Author:
** Creation Time: Wed Sep 20 23:05:15 2017
*/
#ifndef _ARBITRATION_H
#define _ARBITRITION_H

struct task;

enum weight {
	eWeightH = 0,
	eWeightM,
	eWeightL,

	eWeightMax
};

void WRR_setWeight(enum weight w, unsigned int wmax);
void WRR_addTask(struct task *task, enum weight w);
struct task *WRR_getTask(void);

#endif /* _ARBITRATION_H */

