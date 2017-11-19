/******************************************************************************
** File Name: 1038.c
** Author:
** Creation Time: Sun Nov 19 04:14:15 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	150	/* length of plate ranges [1..N] */
#define	M	10	/* height of plate ranges [1..M] */

#define	NS	3
#define	S	59049	/* power(NS, N) */

int	n;
int	m;

char	b[N + 1][M + 1];

static	int dp(void);

int main(void)
{
	int	d;
	int	k;
	int	x, y;

	scanf("%d", &d);
	while (d--) {
		memset(b, 0, sizeof b);

		scanf("%d%d%d", &n, &m, &k);
		while (k--) {
			scanf("%d%d", &x, &y);
			b[x][y] = 1;
		}

		printf("%d\n", dp());
	}

	return 0;
}

static int compress(int s[])
{
	int	cs = 0;
	int	i;

	for (i = m; i > 0; --i)
		cs = cs * NS + s[i];

	return cs;
}

static int decompress(int cs, int s[])
{
	int	i;

	for (i = 1; i <= m; ++i) {
		s[i] = cs % NS;
		cs = cs / NS;
	}
}

#define	max(a, b)	((a) > (b) ? (a) : (b))
#define	min(a, b)	((a) < (b) ? (a) : (b))

int s[2][M + 1];
int f[2][S + 1];
int ns[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049};

void dfs(int cur, int c, int v, int cs)
{
	int	pre = cur ^ 1;
	int	*p = &s[cur][c];
	
	f[cur][cs] = max(f[cur][cs], v);

	if (c + 1 <= m) {
		if (2 == s[pre][c] && 2 == s[pre][c + 1] && 2 == p[0] && 2 == p[1]) {
			p[0] = p[1] = 0;
			dfs(cur, c + 2, v + 1, compress(s[cur]));
			p[0] = p[1] = 2;
		}
		if (c + 2 <= m && 2 == p[0] && 2 == p[1] && 2 == p[2]) {
			p[0] = p[1] = p[2] = 0;
			dfs(cur, c + 3, v + 1, compress(s[cur]));
			p[0] = p[1] = p[2] = 2;
		}
		dfs(cur, c + 1, v, cs);
	}
}

int dp(void)
{
	int	ans;
	int	pre = 0, cur = pre ^ 1;
	int	r, c;
	int	cs;

	r = 1;
	for (c = 1; c <= m; ++c)
		s[cur][c] = b[r][c] ? 0 : 1;

	memset(f[cur], -1, sizeof f[cur]);
	f[cur][compress(s[cur])] = 0;

	for (r = 2; r <= n; ++r) {
		pre = cur;
		cur = pre ^ 1;
		memset(f[cur], -1, sizeof f[cur]);

		for (cs = 0; cs < ns[m]; ++cs) {
			if (f[pre][cs] < 0) continue;
			decompress(cs, s[pre]);
			for (c = 1; c <= m; ++c)
				s[cur][c] = b[r][c] ? 0 : min(2, s[pre][c] + 1);
			dfs(cur, 1, f[pre][cs], compress(s[cur]));
		}
	}

	ans = 0;
	for (cs = 0; cs < ns[m]; ++cs)
		ans = max(ans, f[cur][cs]);

	return ans;
}

