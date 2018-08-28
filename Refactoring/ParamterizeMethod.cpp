#include <limits.h>

extern int lastUsage(void);
extern int min(int, int);

static int usageInRange(int lower, int upper)
{
	if (lastUsage() > lower)
		return min(lastUsage(), upper) - lower;

	return 0;
}

double baseCharge()
{
	double result = usageInRange(0, 100) * 0.03;
	result += usageInRange(100, 200) * 0.05;
	result += usageInRange(200, INT_MAX) * 0.07;

	return result;
}

