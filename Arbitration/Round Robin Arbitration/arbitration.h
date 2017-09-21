/******************************************************************************
** File Name: arbitration.h
** Author:
** Creation Time: Wed Sep 20 23:05:15 2017
*/
#ifndef _ARBITRATION_H
#define _ARBITRITION_H

struct task;

void RR_addTask(struct task *task);
struct task *RR_getTask(void);

#endif /* _ARBITRATION_H */

