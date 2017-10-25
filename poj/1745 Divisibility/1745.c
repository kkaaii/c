/******************************************************************************
** File Name: 1745.c
** Author:
** Creation Time: Wed Oct 25 02:00:09 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	10000
#define	K	100

int n;
int k;
int x[N + 1];

static int dp(void);

int main(void)
{
	int i;
	int v;

	while (~scanf("%d%d", &n, &k)) {
		for (i = 1; i <= n; ++i) {
			scanf("%d", &v);
			x[i] = abs(v) % k;
		}
		puts(dp() ? "Divisible" : "Not divisible");
	}

	return 0;
}

char f[N + 1][K];

#define	MODADD(a,b)	((a) + (b) >= k ? (a) + (b) - k : (a) + (b))	

int dp(void)
{
	int i, j;
	int v = x[1];

	memset(f, 0, sizeof f);
	f[1][v] = 1;

	for (i = 2; i <= n; ++i) {
		v = x[i];
		for (j = 0; j < k; ++j) {
			if (!f[i - 1][j])
				continue;
			f[i][MODADD(j,     v)] = 1;
			f[i][MODADD(j, k - v)] = 1;
		}
	}

	return f[n][0];
}

