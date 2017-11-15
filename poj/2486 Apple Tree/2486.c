/******************************************************************************
** File Name: 2486.c
** Author:
** Creation Time: Wed Nov 15 07:11:51 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	628K	47MS	GCC		1327B	2017-11-15 21:18:01
*/
#include <stdio.h>
#include <string.h>

#define	N	100	/* number of nodes ranges [1..N] */
#define	K	200	/* number of steps ranges [1..K] */

int n;
int k;
int a[N + 1];

int e[N + 1];
int t[N + 1][N + 1];

void add_edge(int u, int v)
{
	t[u][++e[u]] = v;
}

static int dp(void);

int main(void)
{
	int i;
	int u, v;

	while (~scanf("%d%d", &n, &k)) {
		memset(e, 0, sizeof e);

		for (i = 1; i <= n; ++i)
			scanf("%d", &a[i]);

		for (i = 1; i <= n - 1; ++i) {
			scanf("%d%d", &u, &v);
			add_edge(u, v);
			add_edge(v, u);
		}

		printf("%d\n", dp());
	}

	return 0;
}

#define	max(a, b)	((a) > (b) ? (a) : (b))
#define	MAX(a, b)	((a) = max((a), (b)))

/* f[i][j][k] - start from node i, go j steps, return or not return */
int f[N + 1][K + 1][2];

void knapsack(int u, int v)
{
	int i, j;

	for (i = k; i >= 1; --i) {
		for (j = 1; j <= i; ++j) {
			MAX(f[u][i][0], f[u][i - j][1] + f[v][j - 1][0]);
			if (j < 2) continue;
			MAX(f[u][i][0], f[u][i - j][0] + f[v][j - 2][1]);
			MAX(f[u][i][1], f[u][i - j][1] + f[v][j - 2][1]);
		}
	}
}

void dfs(int u, int p)
{
	int i, j, m;
	int v;

	for (i = 0; i <= k; ++i)
		f[u][i][1] = f[u][i][0] = a[u];

	for (i = 1; i <= e[u]; ++i) {
		v = t[u][i];
		if (v == p) continue;
		dfs(v, u);
		knapsack(u, v);
	} 
}

int dp(void)
{
	dfs(1, 0);
	return max(f[1][k][1], f[1][k][0]);
}

