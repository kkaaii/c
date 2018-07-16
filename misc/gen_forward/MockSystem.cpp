#include "CppUTestExt/MockSupport.h"

#include "MockSystem.h"

int MockSystem(const char *command)
{
	return mock("system")
		.actualCall("system")
		.withParameter("command", command)
		.returnValue().getIntValue();
}

		
