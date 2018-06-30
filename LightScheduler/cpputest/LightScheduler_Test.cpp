#include "CppUTest/TestHarness.h"

extern "C" {
#include "LightControllerSpy.h"
#include "FakeTimeService.c"
#include "LightScheduler.c"
}

TEST_GROUP(LightScheduler)
{
	void setup() {
		LightScheduler_Create();
		LightController_Create();
	}

	void teardown() {
		LightController_Destroy();
		LightScheduler_Destroy();
	}

	void TransitionClockTo(int day, int minute) {
		FakeTimeService_SetDay(day);
		FakeTimeService_SetMinute(minute);
		LightScheduler_Wakeup();
	}
};

#define	LIGHTS_ARE_UNCHANGED()						do {	\
	LONGS_EQUAL(LIGHT_ID_NONE, LightControllerSpy_GetLastId());		\
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());	\
} while (0)

#define	THEN_LIGHT_IS_ON(id)					do {	\
	LONGS_EQUAL((id), LightControllerSpy_GetLastId());		\
	LONGS_EQUAL(LIGHT_STATE_ON, LightControllerSpy_GetLastState());	\
} while (0)
	
TEST(LightScheduler, no_lights_controlled_during_creation)
{
	LIGHTS_ARE_UNCHANGED();
}

TEST(LightScheduler, no_lights_controlled_when_none_scheduled)
{
	LightScheduler_Wakeup();
	LIGHTS_ARE_UNCHANGED();
}

TEST(LightScheduler, no_lights_controlled_when_its_not_the_scheduled_time)
{
	LightScheduler_AddTurnOn(3, EVERYDAY, 1200);
	TransitionClockTo(SUNDAY, 1199);
	LIGHTS_ARE_UNCHANGED();
}

TEST(LightScheduler, light_turns_on_at_the_scheduled_time_for_everyday)
{
	LightScheduler_AddTurnOn(3, EVERYDAY, 1200);
	TransitionClockTo(SUNDAY, 1200);
	THEN_LIGHT_IS_ON(3);
}

