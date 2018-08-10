#include "CppUTestExt/MockSupport.h"

#include "MockSystem.h"

int MockSystem(const char *command)
{
	return mock("stdlib")
		.actualCall("MockSystem")
		.withParameter("command", command)
		.returnValue().getIntValue();
}

