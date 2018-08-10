#ifndef	_MOCK_SYSTEM_H
#define	_MOCK_SYSTEM_H

extern "C" {
int MockSystem(const char *command);
const char *MockSystem_GetCommand(void);
}

#endif	/* _MOCK_SYSTEM_H */

