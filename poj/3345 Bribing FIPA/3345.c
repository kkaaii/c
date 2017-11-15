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

int add_country(const char *name)
{
	int i = nc;

	while (i > 0 && strcmp(c[i].name, name))
		--i;

	if (0 == i) {
		i = ++nc;
		strcpy(c[i].name, name);
		c[i].root = TRUE;
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
		nc = 0;

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

#define	min(a, b)	((a) < (b) ? (a) : (b))

int f[N + 1][N + 1];

void dfs(int u)
{
	int i, j, k;
	int v;

	f[u][0] = 0;
	for (i = 1; i <= n; ++i)
		f[u][i] = c[u].cost;

	c[u].vote = 0;
	for (i = head[u]; i > 0; i = next[i]) {
		v = edge[i];
		dfs(v);
		c[u].vote += c[v].vote;

		for (j = c[u].vote; j > 0; --j) {
			for (k = 1; k <= c[v].vote && k <= j; ++k)
				f[u][j] = min(f[u][j], f[u][j - k] + f[v][k]);
		}
	}
	j = ++c[u].vote;
	f[u][j] = c[u].cost;
}

int dp(void)
{
	int i;
	int ans;

	c[0].cost = 0;
	for (i = 1; i <= n; ++i) {
		if (c[i].root) {
			c[0].cost += c[i].cost;
			add_edge(0, i);
		}
	}

	dfs(0);

	ans = f[0][m];
	for (i = m + 1; i <= n; ++i)
		ans = min(ans, f[0][i]);

	return ans;
}

