/******************************************************************************
** File Name: 3181.c
** Author:
** Creation Time: Tue Oct 24 01:46:39 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	1000	/* number of dollars ranges [1..N] */
#define	K	100	/* cost of tools ranges [1..K] */

#define	MOD	1000000000000000000LL

long long b[N + 1][K + 1];
long long a[N + 1][K + 1];

int main(void)
{
	int	n, k;
	int	i, j, m;

	scanf("%d%d", &n, &k);

	memset(b, 0, sizeof b);
	memset(a, 0, sizeof a);

	for (i = 1; i <= n; ++i) {
		b[i][1] = 0;
		a[i][1] = 1;

		for (j = 2; j <= k; ++j) {
			b[i][j] = b[i][j - 1];
			a[i][j] = a[i][j - 1];
			if (j > i) continue;

			for (m = j; m <= i; m += j) {
				if (m == i) {
					++a[i][j];
				} else {
					b[i][j] += b[i - m][j - 1];
					a[i][j] += a[i - m][j - 1];
				}
				b[i][j] += a[i][j] / MOD;
				a[i][j]  = a[i][j] % MOD;
			}
		}
	}

	if (b[n][k]) {
		printf("%lld", b[n][k]);
	}
	printf("%lld\n", a[n][k]);

	return 0;
}

