#ifndef	_LIGHT_CONTROLLER_SPY_H
#define	_LIGHT_CONTROLLER_SPY_H

#include "LightController.h"

enum {
	LIGHT_ID_NONE	= 0
};

enum {
	LIGHT_STATE_OFF	= 0,
	LIGHT_STATE_ON	= 1,
	LIGHT_STATE_UNKNOWN
};

int LightControllerSpy_GetLastId(void);
int LightControllerSpy_GetLastState(void);

#endif	/* _LIGHT_CONTROLLER_SPY_H */

