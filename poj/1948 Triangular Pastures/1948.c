/******************************************************************************
** File Name: 1948.c
** Author:
** Creation Time: Fri Oct 20 02:38:25 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	2940K	125MS	GCC		1285B		2017-10-28 12:00:16
*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#define	N	40	/* number of segments ranges [3..N] */
#define	L	40	/* length of segment ranges [1..L] */

#define	F	100.0	/* factor */

#define	S	(N * L / 2)

int n;
int x[N + 1];	/* length of segments */
int sum;

static int dp(void);

int main(void)
{
	int i;

	while (~scanf("%d", &n)) {
		sum = 0;
		for (i = 1; i <= n; ++i) {
			scanf("%d", &x[i]);
			sum += x[i];
		}
		printf("%d\n", dp());
	}

	return 0;
}

static int area(int a, int b, int c)
{
	double p = (a + b + c) / 2.0;
	return (int)(F * sqrt(p * (p - a) * (p - b) * (p - c)));
}

/*
** 2D Knapsack Problem
**
** f[a][b] - possibility of 2 sides, the length of one is a, of another is b
*/
int f[S + 1][S + 1];

int dp(void)
{
	int i;
	int a, b, c;
	int ans, t;

	memset(f, 0, sizeof f);
	f[0][0] = 1;

	for (i = 1; i <= n; ++i) {
		t = x[i];
		for (a = sum / 2; a >= 0; --a) {
			for (b = a; b >= 0; --b) {
				if ((a >= t && f[a - t][b]) ||
					(b >= t && f[a][b - t])) {
					f[a][b] = 1;
				}
			}
		}
	}

	ans = -1;
	for (a = sum / 2; a > 0; --a) {
		for (b = a; b > 0; --b) {
			if (!f[a][b])
				continue;
			c = sum - a - b;
			if (a + b > c && b + c > a && c + a > b) {
				t = area(a, b, c);
				if (ans < t)
					ans = t;
			}
		}
	}

	return ans;
}

