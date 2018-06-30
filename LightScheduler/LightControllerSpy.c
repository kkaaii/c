#include "LightControllerSpy.h"

static int lastId = LIGHT_ID_NONE;
static int lastState = LIGHT_STATE_UNKNOWN;

int LightControllerSpy_GetLastId(void)
{
	return lastId;
}

int LightControllerSpy_GetLastState(void)
{
	return lastState;
}

void LightController_Create(void)
{
	lastId = LIGHT_ID_NONE;
	lastState = LIGHT_STATE_UNKNOWN;
}

void LightController_Destroy(void)
{
}

void LightController_On(int id)
{
	lastId = id;
	lastState = LIGHT_STATE_ON;
}

void LightController_Off(int id)
{
	lastId = id;
	lastState = LIGHT_STATE_OFF;
}

