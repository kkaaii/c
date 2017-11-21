/******************************************************************************
** File Name: 3735.c
** Author:
** Creation Time: Mon Nov 20 06:18:36 2017
*/
#include <stdio.h>
#include <string.h>

#define	N	100	/* number of cats ranges [1..N] */

typedef struct {
	long long	v[N + 1][N + 1];
} Matrix;

int	n;
int	m;
int	k;

Matrix	a;

#define	each(var)	var = 0; var <= n; ++var
#define	swap(a, b)	do {		\
	long long t = a; a = b; b = t;	\
} while (0)

static void I(Matrix *a)	/* identity */
{
	int	i;

	memset(a, 0, sizeof *a);
	for (each(i))
		a->v[i][i] = 1;
}

Matrix mul(Matrix *a, Matrix *b)
{
	int	i, j, k;
	Matrix	c;

	memset(&c, 0, sizeof c);

	for (each(i)) {
		for (each(k)) {
			if (0 == a->v[i][k]) continue;
			for (each(j))
				c.v[i][j] += a->v[i][k] * b->v[k][j];
		}
	}

	return c;
}

Matrix mpow(Matrix *a, int k)
{
	Matrix	b;

	I(&b);

	while (k) {
		if (k & 1)
			b = mul(&b, a);
		k >>= 1;
		if (k)
			*a = mul(a, a);
	}

	return b;
}

int main(void)
{
	int	i, j;
	int	r;
	char	s[5];
	Matrix	b;

	while (~scanf("%d%d%d", &n, &m, &k) && (n || m || k)) {
		I(&a);

		while (k--) {
			scanf("%s", s);
			switch (s[0]) {
			case 'g':
				scanf("%d", &i);
				++a.v[0][i];
				break;

			case 'e':
				scanf("%d", &i);
				for (each(r))
					a.v[r][i] = 0;
				break;


			case 's':
				scanf("%d%d", &i, &j);
				for (each(r))
					swap(a.v[r][i], a.v[r][j]);
				break;
			}
		}

		b = mpow(&a, m);
		for (r = 1; r <= n; ++r)
			printf("%lld ", b.v[0][r]);
		putchar('\n');
	}

	return 0;
}

