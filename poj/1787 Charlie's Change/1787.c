/******************************************************************************
** File Name: 1787.c
** Author:
** Creation Time: Thu Nov 16 05:01:07 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	552K	94MS	GCC		1151B	2017-11-16 20:23:38
*/
#include <stdio.h>
#include <string.h>

#define	P	10000	/* coffee price in cents ranges [1..P] */
#define	T	4

#define	C	1
#define	N	5
#define	D	10
#define	Q	25

#define	PASS	"Throw in %d cents, %d nickels, %d dimes, and %d quarters."
#define	FAIL	"Charlie cannot buy coffee."

int p;
int n[T];
int w[T] = {C, N, D, Q};

static void dp(void);

int main(void)
{
	while (~scanf("%d%d%d%d%d", &p, &n[0], &n[1], &n[2], &n[3])) {
		if (p || n[0] || n[1] || n[2] || n[3])
			dp();
	}

	return 0;
}

int u[P + 1];
int r[P + 1];

void bounded_knapsack(int f[], int c, int w, int n)
{
	int i;

	memset(u, 0, sizeof u);
	for (i = w; i <= c; ++i) {
		if (u[i - w] < n && f[i - w] >= 0 && f[i] < f[i - w] + 1) {
			u[i] = u[i - w] + 1;
			f[i] = f[i - w] + 1;
			r[i] = i - w;
		}
	}
}

#define	INF	0x7F

int f[P + 1];

void dp(void)
{
	int	i;
	int	ans[Q + 1];

	memset(f, ~INF, sizeof f);
	f[0] = 0;
	r[0] = -1;

	for (i = 0; i < T; ++i)
		bounded_knapsack(f, p, w[i], n[i]);

	if (f[p] < 0) {
		puts(FAIL);
	} else {
		memset(ans, 0, sizeof ans);
		for (i = p; r[i] != -1; i = r[i])
			++ans[i - r[i]];
		printf(PASS "\n", ans[C], ans[N], ans[D], ans[Q]);
	}
}

