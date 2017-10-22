/******************************************************************************
** File Name: 3132.c
** Author:
** Creation Time: Sat Oct 21 23:04:27 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	1120
#define	K	14

int	n;
int k;

static void init(void);
static int dp(void);

int main(void)
{
	init();

	while (~scanf("%d%d", &n, &k) && (n || k)) {
		printf("%d\n", dp());
	}

	return 0;
}

char	prime[N + 1];

void init(void)
{
	int i, j;

	memset(prime, 1, sizeof prime);
	prime[0] = prime[1] = 0;

	for (i = 2; i <= N; ++i) {
		if (!prime[i])
			continue;
		for (j = i + i; j <= N; j += i)
			prime[j] = 0;
	}
}

int f[K + 1][N + 1];

int dp(void)
{
	int i, j, m;

	memset(f, 0, sizeof f);
	f[0][0] = 1;

	for (i = 2; i <= n; ++i) {
		if (!prime[i])
			continue;
		for (m = n; m >= i; --m) {
			for (j = k; j > 0; --j)
				f[j][m] += f[j - 1][m - i];
		}
	}

	return f[k][n];
}

