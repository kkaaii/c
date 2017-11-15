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

int nc;
struct country {
	char	name[S + 1];
	int	root;
	int	vote;
	int	cost;
} c[N + 1];

int find(const char *name)
{
	int i = nc;

	while (--i && strcmp(c[i].name, name)) {
	}

	return i;
}

int add_country(const char *name)
{
	int i = find(name);
	if (0 == i) {
		i = nc++;
		strcpy(c[i].name, name);
		c[i].root = TRUE;
		c[i].vote = 1;
	}

	return i;
}

int t;
int edge[N + 1];
int head[N + 1];
int next[N + 1];

void add_edge(int u, int v)
{
	edge[t] = v;
	next[t] = head[u];
	head[u] = t++;
}

static int dp(void);

int main(void)
{
	int i;
	int u, v;
	int cost;
	char name[S + 1];

	while (2 == scanf("%d%d", &n, &m)) {
		memset(head, 0, sizeof head);
		t = 1;
		nc = 1;

		for (i = 1; i <= n; ++i) {
			scanf("%s%d", name, &cost);
			u = add_country(name);
			c[u].cost = cost;
			while (getchar() != '\n') {
				scanf("%s", name);
				v = add_country(name);
				c[v].root = FALSE;
				add_edge(u, v);
			}
		}

		printf("%d\n", dp());
	}

	return 0;
}

void dfs(int u)
{
	int i;
	int v;

	for (i = head[u]; i > 0; i = next[i]) {
		v = edge[i];
		dfs(v);
		c[u].vote += c[v].vote;
	}
}

#define	min(a, b)	((a) < (b) ? (a) : (b))

void knapsack(int f[], int c, int w, int v)
{
	for (; c >= w; --c)
		f[c] = min(f[c], f[c - w] + v);
}

#define	INF	0x3F

int f[N + 1];

int dp(void)
{
	int i;

	memset(f, INF, sizeof f);
	f[0] = 0;

	for (i = 1; i <= n; ++i) {
		if (c[i].root) {
			dfs(i);
			knapsack(f, N, c[i].vote, c[i].cost);
		}
	}

	for (i = m; i < n && f[i] < m; ++i) {
	}

	return f[i];
}

