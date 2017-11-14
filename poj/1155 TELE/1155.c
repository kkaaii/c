/******************************************************************************
** File Name: 1155.c
** Author:
** Creation Time: Mon Nov 13 07:53:28 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	3000	/* number of vertices in the tree ranges [2..N] */

int n;
int m;
int t = 1;
int head[N + 1] = {0};
int next[N + 1];

struct edge {
	int v;		/* vertex */
	int w;		/* weight */
} e[N + 1];

void add_edge(int u, int v, int w)
{
	e[t].v = v;
	e[t].w = w;
	next[t] = head[u];
	head[u] = t++;
}

static int dp(void);

int main(void)
{
	int i;
	int k;
	int a;
	int c;

	scanf("%d%d", &n, &m);
	for (i = 1; i <= n - m; ++i) {
		scanf("%d", &k);
		while (k--) {
			scanf("%d%d", &a, &c);
			add_edge(i, a, c);
		}
	}

	printf("%d\n", dp());
	return 0;
}

#define	max(a, b)	((a) > (b) ? (a) : (b))

int f[N + 1][N + 1];
int x[N + 1];

int dfs(int u)
{
	int i, j, k;
	int v;

	for (i = head[u]; i > 0; i = next[i]) {
		v = e[i].v;
		dfs(v);

		for (j = x[u]; j >= 0; --j) {
			for (k = 1; k <= x[v]; ++k) {
				f[u][j + k] = max(f[u][j + k], f[u][j] + f[v][k] - e[i].w);
			}
		}
		x[u] += x[v];
	}
}

#define	INF	0x7F

int dp(void)
{
	int i;

	memset(f, ~INF, sizeof f);
	memset(x, 0, sizeof x);
	for (i = 1; i <= n; ++i)
		f[i][0] = 0;

	for (i = n - m + 1; i <= n; ++i) {
		scanf("%d", &f[i][1]);
		x[i] = 1;
	}

	dfs(1);

	for (i = m; i >= 0; --i) {
		if (f[1][i] >= 0)
			break;
	}

	return i;
}

