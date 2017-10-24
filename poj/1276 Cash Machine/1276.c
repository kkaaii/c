/******************************************************************************
** File Name: 1276.c
** Author:
** Creation Time: Tue Oct 24 04:17:49 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	10		/* number of bill denomination ranges [0..N] */

#define	C	100000	/* amount of cash requested ranges [0..C] */

#define	NK	1000	/* number of available bills ranges [0..NK] */
#define	DK	1000	/* value of bill denomination ranges [1..DK] */

int cash;
int k;
int n[N + 1];
int d[N + 1];

static int dp(void);

int main(void)
{
	int i;

	while (~scanf("%d%d", &cash, &k)) {
		for (i = 1; i <= k; ++i)
			scanf("%d%d", &n[i], &d[i]);
		printf("%d\n", dp());
	}

	return 0;
}

int f[C + 1];

int dp(void)
{
	int i;

	memset(f, 0, sizeof f);

	for (i = 1; i <= k; ++i) {
		bounded_knapsack(f, cash, d[i], d[i], n[i]);
	}

	return f[cash];
}

