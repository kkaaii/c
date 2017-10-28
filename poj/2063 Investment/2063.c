/******************************************************************************
** File Name: 2063.c
** Author:
** Creation Time: Tue Oct 24 01:07:52 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	604K	63MS	GCC		984B	2017-10-28 12:26:34
*/
#include <stdio.h>
#include <string.h>

#define	D	10		/* number of bonds ranges [1..D] */
#define	C	1000000	/* start capital ranges [0..C] */
#define	Y	40		/* number of years to grow ranges [0..Y] */

#define	U	1000	/* unit of capital */

#define	M	45300	/* (C/U) * (1+10%)^Y ~= 45259 */

int d;
int w[D + 1];
int v[D + 1];
int capital;

static int dp(void);

int main(void)
{
	int i;
	int n;
	int years;

	scanf("%d", &n);
	while (n--) {
		scanf("%d%d", &capital, &years);
		scanf("%d", &d);
		for (i = 1; i <= d; ++i) {
			scanf("%d%d", &w[i], &v[i]);
			w[i] /= U;
		}

		while (years--)
			capital += dp();

		printf("%d\n", capital);
	}

	return 0;
}

void unbounded_knapsack(int f[], int c, int w, int v)
{
	int i;

	for (i = w; i <= c; ++i) {
		int t = f[i - w] + v;
		if (f[i] < t)
			f[i] = t;
	}
}

int f[M + 1];

int dp(void)
{
	int i;
	int c = capital / U;

	memset(f, 0, sizeof f);

	for (i = 1; i <= d; ++i) {
		unbounded_knapsack(f, c, w[i], v[i]);
	}

	return f[c];
}

