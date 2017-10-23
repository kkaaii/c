/******************************************************************************
** File Name: 3628.c
** Author:
** Creation Time: Mon Oct 23 07:17:06 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	20		/* number of cows ranges [1..N] */
#define	H	1000000	/* height of cows ranges [1..H] */

#define	S	(N * H)	/* sum of height */

int n;	/* number of cows */
int b;	/* height of bookshelf */
int h[N + 1];	/* height of cows */
int s;

int main(void)
{
	int i;

	while (~scanf("%d%d", &n, &b)) {
		s = 0;
		for (i = 1; i <= n; ++i) {
			scanf("%d", &h[i]);
			s += h[i];
		}

		printf("%d\n", dp());
	}

	return 0;
}

int f[S + 1];

int dp(void)
{
	int i;
	int j;

	memset(f, 0, sizeof f);
	f[0] = 1;

	for (i = 1; i <= n; ++i) {
		for (j = s; j >= h[i]; --j)
			f[j] |= f[j - h[i]];
	}

	for (j = b; j <= s; ++j) {
		if (f[j])
			break;
	}

	return j - b;
}

