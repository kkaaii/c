/******************************************************************************
** File Name: 1976.c
** Author:
** Creation Time: Wed Oct 25 00:46:29 2017
*/
#include <stdio.h>
#include <string.h>

#define	L	3		/* number of mini locomotives ranges [1..L] */
#define	N	50000	/* number of coaches ranges [1..N] */
#define	P	100		/* number of passengers per coach ranges [1..P] */

int n;				/* number of coaches */
int	p[N + 1];		/* number of total passengers in first i coaches */
int m;				/* number of coaches a mini locomotive can pull */

static void pre(void);
static int dp(void);

int main(void)
{
	int t;
	int i;

	scanf("%d", &t);
	while (t--) {
		scanf("%d", &n);
		p[0] = 0;
		for (i = 1; i <= n; ++i) {
			scanf("%d", &p[i]);
			p[i] += p[i - 1];
		}
		scanf("%d", &m);

		printf("%d\n", dp());
	}

	return 0;
}

/*
** f[i][j] - scan to the ith coach with j mini locomotives
** 
** f[i][j] = max(f[i - 1][j], f[i - m][j - 1] + v[i])
**
** for example, we have n = 10, and m = 3, f[5][2] is calculated as
** 		[0 1 2] [3 4 5] 6 7 8 9
**
** then f[6][2] could be:
**   1) do not choose coach #6, so f[6][2] = f[5][2]
**   2) do not choose coach #0 or #3, the bigger one is calculated in f[4][1]
**      so f[6][2] = f[4][1] + v[6]
*/
int f[N + 1][L + 1];

#define	max(a,b)	((a) > (b) ? (a) : (b))

int dp(void)
{
	int i, j, k;
	int v;

	memset(f, 0, sizeof f);

	for (i = 1; i <= n; ++i) {
		k = (i <= m) ? 0 : i - m;
		v = p[i] - p[k];
		for (j = 1; j <= L; ++j) {
			f[i][j] = max(f[i - 1][j], f[k][j - 1] + v);
		}
	}

	return f[n][L];
}

