#include "CppUTest/TestHarness.h"

extern "C" {
#include "../LightControllerSpy.c"
}

TEST_GROUP(LightControllerSpy)
{
};

TEST(LightControllerSpy, state_unknown_after_LightController_Create)
{
	LONGS_EQUAL(LIGHT_ID_NONE, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

