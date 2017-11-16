/******************************************************************************
** File Name: 2923.c
** Author:
** Creation Time: Thu Nov 16 04:06:03 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	440K	125MS	GCC		1344B	2017-11-16 17:32:31
*/
#include <stdio.h>
#include <string.h>

#define	N	10	/* number of pieces of furniture ranges [1..N] */
#define	W	100	/* weight of furniture ranges [1..W] */

int n;
int c1, c2;
int w[N];

static int dp(void);

int main(void)
{
	int t;
	int i, j;

	scanf("%d", &t);
	for (i = 1; i <= t; ++i) {
		scanf("%d%d%d", &n, &c1, &c2);
		for (j = 0; j < n; ++j)
			scanf("%d", &w[j]);
		printf("Scenario #%d:\n%d\n\n", i, dp());
	}

	return 0;
}

#define	S	(1 << N)
#define	BIT(x)	(1 << (x))

int f[N * W + 1];
int v[S + 1];	/* valid states */
int nv;		/* number of valid states */

void knapsack(int f[], int c, int w)
{
	for (; c >= w; --c)
		f[c] |= f[c - w];
}

void add_state(int s)
{
	int i;
	int sum = 0;

	memset(f, 0, sizeof f);
	f[0] = 1;

	for (i = 0; i < n; ++i) {
		if (s & BIT(i)) {
			sum += w[i];
			if (sum > c1 + c2) return;
			knapsack(f, c1, w[i]);
		}
	}

	for (i = 0; i <= c1; ++i) {
		if (f[i] && sum - i <= c2) {
			v[nv++] = s;
			break;
		}
	}
}

#define	INF		0x7F
#define	min(a, b)	((a) < (b) ? (a) : (b))

int g[S + 1];

int dp(void)
{
	int i, j;
	int s;
	int c = (1 << n) - 1;

	nv = 0;
	for (s = 1; s <= c; ++s)
		add_state(s);

	memset(g, INF, sizeof g);
	g[0] = 0;

	for (i = 0; i < nv; ++i) {
		for (j = c; j >= 0; --j) {
			if (v[i] & j) continue;
			s = v[i] | j;
			g[s] = min(g[s], g[j] + 1);
		}
	}

	return g[c];
}

