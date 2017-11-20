/******************************************************************************
** File Name: 3233.c
** Author:
** Creation Time: Mon Nov 20 03:42:51 2017
*/
#include <stdio.h>

#define	N	30
#define	N2	(N * 2)

int	n, n2;
int	k;
int	m;

/*
** [ A E] ^ n = [ A ^ n   1 + A + A^2 + ... + A^n]
** [ 0 E]       [ 0       E                      ]
*/
int	a[N2][N2];
int	b[N2][N2];
int	t[N2][N2];

void mul(int c[][N2], int a[][N2], int b[][N2])	/* C = A * B */
{
	int	i, j, k;

	for (i = 0; i < n2; ++i) {
		for (j = 0; j < n2; ++j) {
			t[i][j] = 0;
			for (k = 0; k < n2; ++k) {
				t[i][j] += a[i][k] * b[k][j];
				t[i][j] %= m;
			}
		}
	}

	for (i = 0; i < n2; ++i)
		for (j = 0; j < n2; ++j)
			c[i][j] = t[i][j];
}

int main(void)
{
	int	i, j;
	int	v;

	scanf("%d%d%d", &n, &k, &m);
	n2 = n + n;
	for (i = 0; i < n; ++i) {
		b[i][i] = 1;
		for (j = 0; j < n; ++j)
			scanf("%d", &a[i][j]);

		for (j = 0; j < n; ++j)
			a[i][n + j] = (i == j);
	}

	for (i = n; i < n2; ++i)
		a[i][i] = b[i][i] = 1;

	for (++k; k; k >>= 1) {
		if (k & 1)
			mul(b, b, a);
		mul(a, a, a);
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			v = b[i][n + j] - (i == j);
			if (v < 0) v += m;
			printf("%d ", v);
		}
		putchar('\n');
	}

	return 0;
}

