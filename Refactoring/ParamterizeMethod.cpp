#include <algorithm>

extern int lastUsage(void);
extern int min(int, int);

double baseCharge()
{
	double result = min(lastUsage(), 100) * 0.03;
	if (lastUsage() > 100) {
		result += (min(lastUsage(), 200) - 100) * 0.05;
	}
	if (lastUsage() > 200) {
		result += (lastUsage() - 200) * 0.07;
	}

	return result;
}

