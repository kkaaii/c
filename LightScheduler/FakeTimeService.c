#include "TimeService.h"
#include "FakeTimeService.h"

typedef struct {
	int	day;
	int	minute;
} FakeTimeService;

static FakeTimeService service;

int TimeService_GetMinute(void)
{
	return service.minute;
}

void FakeTimeService_SetDay(int day)
{
	service.day = day;
}

void FakeTimeService_SetMinute(int minute)
{
	service.minute = minute;
}

