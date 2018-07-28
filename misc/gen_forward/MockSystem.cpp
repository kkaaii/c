#include <string.h>
#include "MockSystem.h"

static char buf[256];

const char *MockSystem_GetCommand(void)
{
	return buf;
}

int MockSystem(const char *command)
{
	strcpy(buf, command);
	return 0;
}

