/******************************************************************************
** File Name: 1742.c
** Author:
** Creation Time: Wed Oct 25 03:11:14 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	100		/* number of coin's denomination ranges [1..N] */
#define	M	100000	/* price ranges [1..M] */
#define	A	100000	/* coin's denomination ranges [1..A] */
#define	C	1000	/* number of coins ranges [1..C] */

int n;
int m;
int	a[N + 1];
int c[N + 1];

int dp(void);

int main(void)
{
	int i;

	while (~scanf("%d%d", &n, &m) && (n || m)) {
		for (i = 1; i <= n; ++i)
			scanf("%d", &a[i]);
		for (i = 1; i <= n; ++i)
			scanf("%d", &c[i]);

		printf("%d\n", dp());
	}

	return 0;
}

int f[M + 1];

int dp(void)
{
	int i;
	int ans;

	memset(f, 0, sizeof f);
	for (i = 1; i <= n; ++i) {
		bounded_knapsack(f, m, a[i], a[i], c[i]);
	}

	ans = 0;
	for (i = 1; i <= m; ++i)
		ans += (i == f[i]);

	return ans;
}

