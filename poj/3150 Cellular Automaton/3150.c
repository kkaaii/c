/******************************************************************************
** File Name: 3150.c
** Author:
** Creation Time: Mon Nov 20 02:09:16 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	500

int	n;
int	m;

typedef struct {
	long long	v[N];
} MN;

MN	a, b, x;

MN mul(MN *a, MN *b)
{
	MN	t;
	int	i, j, k;

	memset(&t, 0, sizeof t);

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			k = (i >= j) ? i - j : n + i - j;
			t.v[i] = (t.v[i] + a->v[j] * b->v[k]) % m;
		}
	}

	return t;
}

void mpow(MN *b, MN *a, int k)
{
	memset(b, 0, sizeof *b);
	b->v[0] = 1;

	for (; k; k >>= 1) {
		if (k & 1)
			*b = mul(b, a);
		*a = mul(a, a);
	}
}

int main(void)
{
	int	i;
	int	v;
	int	d, k;

	while (~scanf("%d%d%d%d", &n, &m, &d, &k)) {
		memset(&a, 0, sizeof a);
		memset(&b, 0, sizeof b);

		a.v[0] = 1;
		for (i = 1; i <= d; ++i)
			a.v[i] = a.v[n - i] = 1;

		mpow(&b, &a, k);

		for (i = 0; i < n; ++i) {
			scanf("%d", &v);
			x.v[i] = v;
		}

		x = mul(&x, &b);

		for (i = 0; i < n; ++i)
			printf("%d ", (int)x.v[i]);
		putchar('\n');
	}

	return 0;
}

