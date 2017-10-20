/******************************************************************************
** File Name: 2184.c
** Author:
** Creation Time: Fri Oct 20 07:05:00 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	100		/* number of cows ranges [1..N] */

#define	S	1000	/* smartness ranges [-S..S] */
#define	F	1000	/* funness ranges [-F..F] */

#define	Z	(N * S)

int n;
int w[N + 1];		/* weight = smartness */
int v[N + 1];		/* value = funness */

static int dp(void);

int main(void)
{
	int i;

	while (~scanf("%d", &n)) {
		for (i = 1; i <= n; ++i)
			scanf("%d%d", &w[i], &v[i]);
		printf("%d\n", dp());
	}

	return 0;
}

#define	max(a, b)	((a) > (b) ? (a) : (b))

int buf[Z + 1 + Z], *f = &buf[Z];

int dp(void)
{
	int i;
	int c;
	int ans;

	memset(buf, 0x80, sizeof buf);
	f[0] = 0;

	for (i = 1; i <= n; ++i) {
		if (w[i] > 0) {
			for (c = Z; c >= w[i] - Z; --c) {
				f[c] = max(f[c], f[c - w[i]] + v[i]);
			}
		} else {
			for (c = -Z; c <= w[i] + Z; ++c) {
				f[c] = max(f[c], f[c - w[i]] + v[i]);
			}
		}
	}

	ans = 0;
	for (c = 0; c <= Z; ++c) {
		if (f[c] >= 0 && ans < f[c] + c)
			ans = f[c] + c;
	}

	return ans;
}

