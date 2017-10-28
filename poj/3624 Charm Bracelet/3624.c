/******************************************************************************
** File Name: 3624 Charm Bracelet/3624.c
** Author:
** Creation Time: Mon Oct 23 06:49:16 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	504K	157MS	GCC		740B	2017-10-28 12:39:49
*/
#include <stdio.h>
#include <string.h>

#define	N	3402	/* number of charms ranges [1..N] */
#define	M	12880	/* weight limits ranges [1..M] */

#define	W	400		/* weight ranges [1..W] */
#define	D	100		/* desirability factor ranges [1..D] */

int n;
int m;
int w[N + 1];
int d[N + 1];

static int dp(void);

int main(void)
{
	int	i;

	while (~scanf("%d%d", &n, &m)) {
		for (i = 1; i <= n; ++i)
			scanf("%d%d", &w[i], &d[i]);
		printf("%d\n", dp());
	}

	return 0;
}

int f[M + 1];

void knapsack(int f[], int c, int w, int v)
{
	for (; c >= w; --c) {
		int t = f[c - w] + v;
		if (f[c] < t)
			f[c] = t;
	}
}

int dp(void)
{
	int i, k;

	memset(f, 0, sizeof f);
	for (i = 1; i <= n; ++i) {
		knapsack(f, m, w[i], d[i]);
	}

	return f[m];
}
