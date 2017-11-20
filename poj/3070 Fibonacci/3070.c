/******************************************************************************
** File Name: 3070.c
** Author:
** Creation Time: Mon Nov 20 00:45:36 2017
*/
#include <stdio.h>

#define	MOD	10000

typedef struct {
	int	v[2][2];
} M22;

#define	a11	a.v[0][0]
#define	a12	a.v[0][1]
#define a21	a.v[1][0]
#define	a22	a.v[1][1]

#define	b11	b.v[0][0]
#define	b12	b.v[0][1]
#define b21	b.v[1][0]
#define	b22	b.v[1][1]

#define	c11	c.v[0][0]
#define	c12	c.v[0][1]
#define c21	c.v[1][0]
#define	c22	c.v[1][1]

M22 mmul(M22 a, M22 b)
{
	M22	c;

	c11 = (a11 * b11 + a12 * b21) % MOD;
	c12 = (a11 * b12 + a12 * b22) % MOD;
	c21 = (a21 * b11 + a22 * b21) % MOD;
	c22 = (a21 * b12 + a22 * b22) % MOD;

	return c;
}

M22 mpow(M22 a, int n)
{
	M22	b;

	if (0 == n) {
		b11 = 0;
	} else {
		b11 = 1, b12 = 0, b21 = 0, b22 = 1;
		for (--n; n > 0; n >>= 1) {
			if (n & 1) b = mmul(a, b);
			a = mmul(a, a);
		}
	}

	return b;
}

int main(void)
{
	int	n;
	M22	a, b;

	a11 = 1, a12 = 1, a21 = 1, a22 = 0;

	while (~scanf("%d", &n) && -1 != n) {
		b = mpow(a, n);
		printf("%d\n", b11);
	}

	return 0;
}

