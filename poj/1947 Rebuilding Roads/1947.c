/******************************************************************************
** File Name: 1947.c
** Author:
** Creation Time: Tue Nov 14 02:44:30 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	150	/* number of barns ranges [1..N] */

int n;
int p;
int t = 1;
int e[N + 1];
int next[N + 1];
int head[N + 1] = {0};

int num[N + 1];

void add_edge(int u, int v)
{
	e[t] = v;
	next[t] = head[u];
	head[u] = t++;
}

static int dp(void);

int main(void)
{
	int i;
	int u, v;

	scanf("%d%d", &n, &p);
	memset(num, 0, sizeof num);
	for (i = 2; i <= n; ++i) {
		scanf("%d%d", &u, &v);
		add_edge(u, v);
		++num[u];
	}

	printf("%d\n", dp());
	return 0;
}

#define	min(a, b)	((a) < (b) ? (a) : (b))

int f[N + 1][N + 1];
int x[N + 1];

void dfs(int u)
{
	int i, j, k;
	int v;

	x[u] = 1;
	if (0 == head[u]) {
		f[u][1] = 0;
		return;
	}

	for (i = head[u]; i > 0; i = next[i]) {
		v = e[i];
		dfs(v);
		x[u] += x[v];
		for (j = x[u]; j > 0; --j) {
			for (k = 1; k < j; ++k) {
				f[u][j] = min(f[u][j], f[u][j - k] + f[v][k] - 1);
			}
		}
	}
}

#define	INF	0x7F

int dp(void)
{
	int ans;
	int i;

	memset(f, INF, sizeof f);

	for (i = 1; i <= n; ++i)
		f[i][1] = num[i];

	dfs(1);

	ans = f[1][p];
	for (i = 2; i <= n; ++i)
		ans = min(ans, f[i][p]);
	return ans;
}

