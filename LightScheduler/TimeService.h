#ifndef	_TIME_SERVICE_H
#define	_TIME_SERVICE_H

enum {
	SUNDAY	= 0,
	MONDAY,
	TUESDAY,
	WENDESDAY,
	THURSDAY,
	FRIDAY,
	SATADAY,
	EVERYDAY = -1
};

int TimeService_GetMinute(void);

#endif	/* _TIME_SERVICE_H */

