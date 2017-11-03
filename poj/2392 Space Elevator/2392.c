/******************************************************************************
** File Name: 2392.c
** Author:
** Creation Time: Fri Nov  3 02:03:28 2017
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	K	400		/* type of blocks ranges [1..K] */
#define	H	100		/* height of block ranges [1..H] */
#define	C	10		/* quantity of block ranes [1..C] */
#define	A	40000	/* allowed altitude of block ranges [1..A] */

struct block {
	int	h;	/* height */
	int a;	/* altitude */
	int c;	/* quantity */
};

int k;
struct block b[K + 1];

static int dp(void);

static cmp(const void *p1, const void *p2)
{
	int x1 = ((struct block *)p1)->a;
	int x2 = ((struct block *)p2)->a;

	return (x1 < x2) ? -1 : (x1 > x2);
}

int main(void)
{
	int i;

	scanf("%d", &k);
	for (i = 1; i <= k; ++i)
		scanf("%d%d%d", &b[i].h, &b[i].a, &b[i].c);
	qsort(b, k, sizeof b[0], cmp);

	printf("%d\n", dp());
	return 0;
}

#define	max(a,b)	((a) > (b) ? (a) : (b))

void knapsack(int f[], int c, int w, int v)
{
	for (; c >= w; --c)
		f[c] = max(f[c], f[c - w] + v);
}

void unbounded_knapsack(int f[], int c, int w, int v)
{
	int i;

	for (i = w; i <= c; ++i)
		f[i] = max(f[i], f[i - w] + v);
}

void bounded_knapsack(int f[], int c, int w, int v, int n)
{
	int k;

	if (w * n >= c) {
		unbounded_knapsack(f, c, w, v);
	} else {
		for (k = 1; k < n; k <<= 1) {
			knapsack(f, c, w * k, v * k);
			n -= k;
		}
		if (0 != n)
			knapsack(f, c, w * n, v * n);
	}
}

int f[A + 1];

int dp(void)
{
	int i;

	memset(f, 0, sizeof f);

	for (i = 1; i <= k; ++i)
		bounded_knapsack(f, b[i].a, b[i].h, b[i].h, b[i].c);

	return f[b[k].a];
}

