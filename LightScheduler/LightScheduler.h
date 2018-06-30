#ifndef _LIGHT_SCHEDULER_H
#define _LIGHT_SCHEDULER_H

void LightScheduler_Create(void);
void LightScheduler_Destroy(void);
void LightScheduler_Wakeup(void);
void LightScheduler_AddTurnOn(int id, int day, int minutes);

#endif	/* _LIGHT_SCHEDULER_H */

