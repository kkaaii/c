/******************************************************************************
** File Name: 3345.c
** Author:
** Creation Time: Tue Nov 14 04:40:29 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	200	/* number of countries ranges [1..N] */
#define	S	100	/* number of characters in name ranges [1..S] */

#define	FALSE	0
#define	TRUE	(!FALSE)

int n;
int m;

char names[N + 1][S + 1];
int d[N + 1];
int v[N + 1];

int find(const char *name, int i)
{
	while (--i && strcmp(names[i], name)) {
	}

	return i;
}

int t = 1;
int edge[N + 1];
int head[N + 1];
int next[N + 1];
int root[N + 1];

void add_edge(int u, int v)
{
	edge[t] = v;
	next[t] = head[u];
	head[u] = t++;
}

static int dp(void);

int main(void)
{
	int i, j;
	char name[S + 1];

	while (2 == scanf("%d%d", &n, &m)) {
		for (i = 1; i <= n; ++i) {
			scanf("%s%d", names[i], &d[i]);
			root[i] = TRUE;
			v[i] = 1;
			while (getchar() != '\n') {
				scanf("%s", name);
				j = find(name, i);
				root[j] = FALSE;
				add_edge(i, j);
			}
		}

		printf("%d\n", dp());
	}

	return 0;
}

void dfs(int i)
{
	int e;
	int j;

	for (e = head[i]; e > 0; e = next[e]) {
		j = edge[e];
		dfs(j);
		v[i] += v[j];
	}
}

#define	min(a, b)	((a) < (b) ? (a) : (b))

void knapsack(int f[], int c, int w, int v)
{
	for (; c >= w; --c)
		f[c] = min(f[c], f[c - w] + v);
}

#define	INF	0x7F

int f[N + 1];

int dp(void)
{
	int i;

	memset(f, INF, sizeof f);
	f[0] = 0;

	for (i = 1; i <= n; ++i) {
		if (root[i]) {
			dfs(i);
			knapsack(f, N, v[i], d[i]);
		}
	}

	for (i = m; i < n && f[i] < m; ++i) {
	}

	return f[i];
}

