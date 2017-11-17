/******************************************************************************
** File Name: 3260.c
** Author:
** Creation Time: Fri Nov 17 00:42:15 2017
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	620K	79MS	GCC		1578B	2017-11-17 13:59:02
*/
#include <stdio.h>
#include <string.h>

#define	N	100	/* type of different coins ranges [1..N] */
#define	T	10000	/* total price of supplies ranges [1..T] */
#define	V	120	/* value of coins ranges [1..V] */

int n;
int t;
int v[N + 1];
int c[N + 1];

#define	max(a, b)	((a) > (b) ? (a) : (b))
#define	min(a, b)	((a) < (b) ? (a) : (b))

static int dp(int m);

int main(void)
{
	int	i;
	int	m;

	while (~scanf("%d%d", &n, &t)) {
		m = 0;
		for (i = 1; i <= n; ++i) {
			scanf("%d", &v[i]);
			m = max(m, v[i]);
		}

		for (i = 1; i <= n; ++i)
			scanf("%d", &c[i]);

		printf("%d\n", dp(m * m + t));
	}

	return 0;
}

static void knapsack(int f[], int c, int w, int v)
{
	for (; c >= w; --c)
		f[c] = min(f[c], f[c - w] + v);
}

static void unbounded_knapsack(int f[], int c, int w, int v)
{
	int	i;

	for (i = w; i <= c; ++i)
		f[i] = min(f[i], f[i - w] + v);
}

static void bounded_knapsack(int f[], int c, int w, int v, int n)
{
	int	k;

	if (w * n >= c) {
		unbounded_knapsack(f, c, w, v);
	} else {
		for (k = 1; k <= n; k <<= 1) {
			knapsack(f, c, w * k, v * k);
			n -= k;
		}
		if (n)  knapsack(f, c, w * n, v * n);
	}
}

#define	INF	0x3F3F3F3F

int b[V * V + T + 1];	/* buyer  */
int s[V * V + T + 1];	/* seller */

int dp(int m)
{
	int	i;
	int	ans;

	memset(b, INF, sizeof b);
	b[0] = 0;

	for (i = 1; i <= n; ++i)
		bounded_knapsack(b, m, v[i], 1, c[i]);

	memset(s, INF, sizeof s);
	s[0] = 0;

	for (i = 1; i <= n; ++i)
		unbounded_knapsack(s, m, v[i], 1);

	ans = INF;
	for (i = t; i <= m; ++i)
		ans = min(ans, b[i] + s[i - t]);

	return (INF == ans) ? -1 : ans;
}

