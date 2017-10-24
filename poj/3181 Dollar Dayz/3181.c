/******************************************************************************
** File Name: 3181.c
** Author:
** Creation Time: Tue Oct 24 01:46:39 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	1000	/* number od dollars ranges [1..N] */
#define	K	100		/* cost of tools ranges [1..K] */

int n;
int k;

int main(void)
{
	scanf("%d%d", &n, &k);
	printf("%d\n", dp());

	return 0;
}

int f[N + 1];

void unbounded_knapsack(int f[], int c, int w, int v)
{
	int i;

	for (i = w; i <= c; ++i) {
		int t = f[i - w] + v;
		if (f[c] < t)
			f[c] = t;
	}
}

int dp(void)
{
	int w;
	int c;

	memset(f, 0, sizeof f);
	f[0] = 1;

	for (w = 1; w <= k; ++w) {
		for (c = w; c <= n; ++c)
			f[c] += f[c - w];
	}

	return f[n];
}

