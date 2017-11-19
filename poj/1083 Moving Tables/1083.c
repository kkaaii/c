/******************************************************************************
** File Name: 1083.c
** Author:
** Creation Time: Sun Nov 19 03:09:43 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	388K	0MS	GCC		640B	2017-11-19 16:38:27
*/
#include <stdio.h>
#include <string.h>

#define	N	200	/* number of tables to move ranges [1..N] */

int	c[N + 1];	/* corridor */

#define	SWAP(a, b)	do { a ^= b; b ^= a; a ^= b; } while (0)
#define	max(a, b)	((a) > (b) ? (a) : (b))

static int dp(void);

int main(void)
{
	int	T;
	int	n;
	int	s, t;
	int	i;

	scanf("%d", &T);
	while (T--) {
		memset(c, 0, sizeof c);
		scanf("%d", &n);
		while (n --) {
			scanf("%d%d", &s, &t);
			s = (s - 1) / 2;
			t = (t - 1) / 2;
			if (s > t) SWAP(s, t);
			for (i = s; i <= t; ++i)
				++c[i];
		}

		n = 0;
		for (i = 0; i <= N; ++i)
			n = max(n, c[i]);

		printf("%d\n", n * 10);
	}

	return 0;
}
