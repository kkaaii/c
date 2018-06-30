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
};

TEST(LightScheduler, no_lights_controlled_during_creation)
{
	LONGS_EQUAL(LIGHT_ID_NONE, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, no_lights_controlled_when_none_scheduled)
{
	LightScheduler_Wakeup();
	LONGS_EQUAL(LIGHT_ID_NONE, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, lights_turned_on_at_the_right_time_scheduled_everyday)
{
	LightScheduler_AddTurnOn(3, EVERYDAY, 1200);
	FakeTimeService_SetDay(SUNDAY);
	FakeTimeService_SetMinute(1200);
	LightScheduler_Wakeup();
	LONGS_EQUAL(3, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_ON, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, no_lights_controlled_when_its_not_the_scheduled_time)
{
	LightScheduler_AddTurnOn(3, EVERYDAY, 1200);
	FakeTimeService_SetDay(SUNDAY);
	FakeTimeService_SetMinute(1200 - 1);
	LightScheduler_Wakeup();
	LONGS_EQUAL(LIGHT_ID_NONE, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, light_turns_on_at_the_scheduled_time_for_everyday)
{
	LightScheduler_AddTurnOn(3, EVERYDAY, 1200);
	FakeTimeService_SetDay(SUNDAY);
	FakeTimeService_SetMinute(1200);
	LightScheduler_Wakeup();
	LONGS_EQUAL(3, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_ON, LightControllerSpy_GetLastState());
}

