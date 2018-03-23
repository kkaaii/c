float InvSqrt(float x)
{
	float	xhalf = x * 0.5f;
	int	*p = (int *)&x;

	*p = 0x5f3759df - (*p >> 1);
	return x * (1.5f - xhalf * x * x);
}

#include <stdio.h>
#include <math.h>

int main(void)
{
	float	x = 1.0f;
	int	i;

	for (i = 0; i < 20; ++i) {
		printf("%.10f\t%.10f\n", 1.0f/sqrt(x), InvSqrt(x));
		x *= 10.0f;
	}

	return 0;
}

