/******************************************************************************
** File Name: 3181.c
** Author:
** Creation Time: Tue Oct 24 01:46:39 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	444K	0MS	GCC		702B	2017-11-17 16:39:13
*/
#include <stdio.h>
#include <string.h>

#define	N	1000	/* number of dollars ranges [1..N] */

#define	MOD	1000000000000000000LL

long long b[N + 1];
long long a[N + 1];

int main(void)
{
	int	n, k;
	int	i, j, m;

	scanf("%d%d", &n, &k);

	memset(b, 0, sizeof b);
	memset(a, 0, sizeof a);
	a[0] = 1;

	for (i = 1; i <= k; ++i) {
		for (j = i; j <= n; ++j) {
			b[j] += b[j - i];
			a[j] += a[j - i];
			b[j] += a[j] / MOD;
			a[j]  = a[j] % MOD;
		}
	}

	if (b[n]) {
		printf("%lld", b[n]);
	}
	printf("%lld\n", a[n]);

	return 0;
}

