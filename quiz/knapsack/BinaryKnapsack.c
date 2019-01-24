/*
** Image there are 5 separated tasks that need to be done in a 
** dual-core system (Core A and Core B). The time to complete each task is
** 2, 6, 7, 9, 12 seconds respectively. Please design an algorithm to
** distribute tasks between two cores in the way that the total running
** time is the shortest. You MUST describe clearly your algorithm in
** pesudo code.
**
** Notice: Each task is independent, and the delay time for next task can
** be ignored.
*/
#include <stdio.h>
#include <string.h>

#define	MAXV	32

typedef struct {
	int	volume;
	int	best[MAXV];
} Knapsack;

void BinaryKnapsack(Knapsack *k, int cost, int value)
{
	int v;

	for (v = k->volume; v >= cost; --v) {
		if (k->best[v] < k->best[v - cost] + value)
			k->best[v] = k->best[v - cost] + value;
	}
}

int main(void)
{
	Knapsack k;
	int	data[] = {2, 6, 7, 9, 12};
	int	sum = 0;
	int	i;

	memset(&k, 0, sizeof k);
	for (i = 0; i < sizeof data / sizeof data[0]; ++i) {
		sum += data[i];
	}
	k.volume = sum / 2;
	
	for (i = 0; i < sizeof data / sizeof data[0]; ++i) {
		BinaryKnapsack(&k, data[i], data[i]);
	}
	
	printf("best = %d\n", sum - k.best[k.volume]);
	
	return 0;
}
