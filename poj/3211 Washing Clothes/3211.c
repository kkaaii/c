/******************************************************************************
** File Name: 3211.c
** Author:
** Creation Time: Tue Nov  7 01:40:31 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	824K	16MS	GCC		1219B	2017-11-07 19:00:46
*/
#include <stdio.h>
#include <string.h>

#define	M	10	/* number of colors ranges [1..M) */
#define	N	100	/* number of clothes ranges [1..N) */

#define	S	10	/* number of characters ranges [1..S) */
#define	T	1000	/* time to wash ranges [1..T) */

int m;
int n;
int x[M + 1];
int s[M + 1];
int t[M + 1][N + 1];

static int dp(void);

int main(void)
{
	char names[M + 1][S + 1];
	char name[S + 1];
	int time;
	int i, j;

	while (~scanf("%d%d", &m, &n) && (m || n)) {
		memset(x, 0, sizeof x);
		memset(s, 0, sizeof s);

		for (j = 0; j < m; ++j)
			scanf("%s", names[j]);

		for (i = 1; i <= n; ++i) {
			scanf("%d%s", &time, name);
			for (j = 0; j < m; ++j) {
				if (0 == strcmp(name, names[j]))
					break;
			}
			t[j][x[j]++] = time;
			s[j] += time;
		}

		printf("%d\n", dp());
	}

	return 0;
}

#define	max(a,b)	((a) > (b) ? (a) : (b))

static inline void knapsack(int f[], int c, int w, int v)
{
	for (; c >= w; --c)
		f[c] = max(f[c], f[c - w] + v);
}

static int f[N * T + 1];

int dp(void)
{
	int sum = 0;
	int i, j;
	int c;

	for (i = 0; i < m; ++i) {
		memset(f, 0, sizeof f);
		c = s[i] / 2;
		for (j = 0; j < x[i]; ++j) {
			knapsack(f, c, t[i][j], t[i][j]);
		}

		sum += s[i] - f[c];
	}

	return sum;
}

