#include "LightScheduler.h"
#include "LightController.h"

typedef struct {
	int	minute;
} ScheduledLightEvent;

static ScheduledLightEvent event;

void LightScheduler_Create(void)
{
	event.minute = -1;
}

void LightScheduler_Destroy(void)
{
}

void LightScheduler_Wakeup(void)
{
	if (TimeService_GetMinute() == event.minute) {
		LightController_On(3);
	}
}

void LightScheduler_AddTurnOn(int id __attribute__((unused)), int day __attribute__((unused)), int minute)
{
	event.minute = minute;
}

