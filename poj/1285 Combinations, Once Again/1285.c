/******************************************************************************
** File Name: 1285.c
** Author:
** Creation Time: Fri Nov  3 01:02:27 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	428K	0MS	GCC		872B	2017-11-03 13:43:40
*/
#include <stdio.h>
#include <string.h>

#define	N	50	/* number of objects ranges [0..N] */

int n;
int m;
int c[N + 1];


static long long f[N + 1];
static void dp(void);

int main(void)
{
	int i;
	int k;
	int t = 0;

	while (~scanf("%d%d", &n, &m) && (n || m)) {
		memset(c, 0, sizeof c);
		for (i = 0; i < n; ++i) {
			scanf("%d", &k);
			++c[k];
		}

		dp();

		printf("Case %d:\n", ++t);
		for (i = 0; i < m; ++i) {
			scanf("%d", &k);
			printf("%lld\n", f[k]);
		}
	}
}

void dp(void)
{
	int i, j, k;

	memset(f, 0, sizeof f);
	for (i = 0; i <= c[1]; ++i)
		f[i] = 1;

	for (i = 2; i <= n; ++i) {
		for (j = n; j >= 1; --j) {
			k = j <= c[i] ? 0 : j - c[i];
			for (; k < j; ++k)
				f[j] += f[k];
		}
	}
}

