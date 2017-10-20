/******************************************************************************
** File Name: 1837.c
** Author:
** Creation Time: Fri Oct 20 01:24:59 2017
*/
#include <stdio.h>
#include <string.h>

#define	C	20	/* number of hooks ranges [2..C] */
#define	G	20	/* number of weights ranges [2..G] */
#define	X	15	/* position of hooks ranges [-X..X] */
#define	W	25	/* value of weights ranges [1..W] */

#define	Z	(G*X*W)	/* torque ranges [-Z..Z] */

int c, g;
int t[G + 1][C + 1];	/* torques */

static int dp(void);

int main(void)
{
	int i, j;
	int x[C + 1];
	int w;

	while (~scanf("%d%d", &c, &g)) {
		for (j = 1; j <= c; ++j)
			scanf("%d", &x[j]);
		for (i = 1; i <= g; ++i) {
			scanf("%d", &w);
			for (j = 1; j <= c; ++j)
				t[i][j] = w * x[j];
		}
		printf("%d\n", dp());
	}

	return 0;
}

/*
** f[i][j] - number of solutions for first i weights with torque j
** f[i][j] = sigma(f[i - 1][j - t[i][1..c])
*/

int f[G + 1][Z + 1 + Z];

int dp(void)
{
	int i, j, k;

	memset(f, 0, sizeof f);
	for (k = 1; k <= c; ++k)
		f[1][Z + t[1][k]] = 1;

	for (i = 2; i <= g; ++i) {
		for (j = Z; j >= -Z; --j) {
			for (k = 1; k <= c; ++k) {
				f[i][Z + j] += f[i - 1][Z + j - t[i][k]];
			}
		}
	}

	return f[g][Z];
}

