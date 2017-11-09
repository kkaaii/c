/******************************************************************************
** File Name: 3046.c
** Author:
** Creation Time: Tue Nov  7 06:17:30 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	496K	47MS	GCC		989B	2017-11-09 09:50:04
*/
#include <stdio.h>
#include <string.h>

#define	T	1000	/* number of families ranges [1..T] */
#define	N	100	/* number of ants in a family ranges [1..N] */

#define	MOD	1000000

int t;
int s;
int b;

int num[T + 1];

static int dp(void);

int main(void)
{
	int a;
	int g;

	scanf("%d%d%d%d", &t, &a, &s, &b);
	memset(num, 0, sizeof num);
	while (a--) {
		scanf("%d", &g);
		++num[g];
	}

	printf("%d\n", dp());

	return 0;
}

#define	swap(a,b)	do {		\
	int *x = a; a = b; b = x;	\
} while (0)

int sum[T * N + 1];
int f[2][MOD + 1];

int dp(void)
{
	int ans;
	int *pre = f[0];
	int *cur = f[1];
	int c = 0;
	int i, j;

	f[0][0] = 1;
	for (i = 1; i <= t; ++i) {
		c += num[i];

		sum[0] = pre[0];
		for (j = 1; j <= c; ++j)
			sum[j] = (sum[j - 1] + pre[j]) % MOD;

		for (j = 0; j <= c; ++j) {
			cur[j] = (j > num[i]) ?
				(sum[j] - sum[j - num[i] - 1] + MOD) % MOD :
				sum[j];
		}
		swap(pre, cur);
	}

	ans = 0;
	for (i = s; i <= b; ++i)
		ans = (ans + pre[i]) % MOD;

	return ans;
}
