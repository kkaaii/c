/******************************************************************************
** File Name: 24.c
** Author:
** Creation Time: Sun Jan  7 00:29:48 2018
*/
#include <stdio.h>

#define	N	4

#define	ABS(x)	((x) < 0 ? -(x) : (x))
#define	DIM(x)	(sizeof (x) / sizeof (x)[0])

#ifdef	DEBUG
#define	DBGMSG	printf
#else
#define	DBGMSG(...)
#endif

typedef struct {
	int	numerator;
	int	denominator;
} Rational;

int gcd(int a, int b)
{
	int	t;

	if (a < b) {
		t = a; 
		a = b;
		b = t;
	}

	while (b) {
		t = a % b;
		a = b;
		b = t;
	}

	return a;
}

void reduction(Rational *a)
{
	int	x;

	DBGMSG("\t%s(%d/%d)", __FUNCTION__, a->numerator, a->denominator);

	if (0 == a->numerator) {
		if (0 != a->denominator)
			a->denominator = 1;
	} else if (0 == a->denominator) {
		a->numerator = 1;
	} else {
		if (a->denominator < 0) {
			a->numerator = -a->numerator;
			a->denominator = -a->denominator;
		}
		x = gcd(ABS(a->numerator), a->denominator);
		a->numerator /= x;
		a->denominator /= x;
	}

	DBGMSG(" = %d/%d\n", a->numerator, a->denominator);
}

int equ(Rational *a, int b)
{
	return (1 == a->denominator) && (b == a->numerator);
}

void add(Rational *a, Rational *b, Rational *res)
{
	DBGMSG("\t%d/%d + %d/%d", a->numerator, a->denominator,
		b->numerator, b->denominator);

	res->denominator = a->denominator * b->denominator;
	res->numerator = a->numerator * b->denominator +
			b->numerator * a->denominator;

	DBGMSG(" = %d/%d\n", res->numerator, res->denominator);
	reduction(res);
}

void sub(Rational *a, Rational *b, Rational *res)
{
	DBGMSG("\t%d/%d - %d/%d", a->numerator, a->denominator,
		b->numerator, b->denominator);

	res->denominator = a->denominator * b->denominator;
	res->numerator = a->numerator * b->denominator -
			b->numerator * a->denominator;

	DBGMSG(" = %d/%d\n", res->numerator, res->denominator);
	reduction(res);
}

void mul(Rational *a, Rational *b, Rational *res)
{
	DBGMSG("\t%d/%d * %d/%d", a->numerator, a->denominator,
		b->numerator, b->denominator);

	res->denominator = a->denominator * b->denominator;
	res->numerator = a->numerator * b->numerator;

	DBGMSG(" = %d/%d\n", res->numerator, res->denominator);
	reduction(res);
}

void div(Rational *a, Rational *b, Rational *res)
{
	DBGMSG("\t%d/%d / %d/%d", a->numerator, a->denominator,
		b->numerator, b->denominator);

	res->denominator = a->denominator * b->numerator;
	res->numerator = a->numerator * b->denominator;

	DBGMSG(" = %d/%d\n", res->numerator, res->denominator);
	reduction(res);
}

struct op {
	char	op;
	void	(*f)(Rational *a, Rational *b, Rational *res);
} ops[] = {
	{.op = '+', .f = add},
	{.op = '-', .f = sub},
	{.op = '*', .f = mul},
	{.op = '/', .f = div}
};

/* (a + b) + (c + d) */
static void expr1(int x[], struct op *op1, struct op *op2, struct op *op3)
{
	Rational a = { .numerator = x[0],  .denominator = 1};
	Rational b = { .numerator = x[1],  .denominator = 1};
	Rational c = { .numerator = x[2],  .denominator = 1};
	Rational d = { .numerator = x[3],  .denominator = 1};
	Rational t1, t2, res;

	(*op1->f)(&a, &b, &t1);
	(*op3->f)(&c, &d, &t2);
	(*op2->f)(&t1, &t2, &res);

	if (equ(&res, 24)) {
		printf("(%d %c %d) %c (%d %c %d) = 24\n",
			x[0], op1->op, x[1], op2->op, x[2], op3->op, x[3]);
	}
}

/* a + ((b + c) + d) */
static void expr2(int x[], struct op *op1, struct op *op2, struct op *op3)
{
	Rational a = { .numerator = x[0],  .denominator = 1};
	Rational b = { .numerator = x[1],  .denominator = 1};
	Rational c = { .numerator = x[2],  .denominator = 1};
	Rational d = { .numerator = x[3],  .denominator = 1};
	Rational t1, t2, res;

	(*op2->f)(&b, &c, &t1);
	(*op3->f)(&t1, &d, &t2);
	(*op1->f)(&a, &t2, &res);

	if (equ(&res, 24)) {
		printf("%d %c ((%d %c %d) %c %d) = 24\n",
			x[0], op1->op, x[1], op2->op, x[2], op3->op, x[3]);
	}
}

/* a + (b + (c + d)) */
static void expr3(int x[], struct op *op1, struct op *op2, struct op *op3)
{
	Rational a = { .numerator = x[0],  .denominator = 1};
	Rational b = { .numerator = x[1],  .denominator = 1};
	Rational c = { .numerator = x[2],  .denominator = 1};
	Rational d = { .numerator = x[3],  .denominator = 1};
	Rational t1, t2, res;

	(*op3->f)(&c, &d, &t2);
	(*op2->f)(&b, &t2, &t1);
	(*op1->f)(&a, &t1, &res);

	if (equ(&res, 24)) {
		printf("%d %c (%d %c (%d %c %d)) = 24\n",
			x[0], op1->op, x[1], op2->op, x[2], op3->op, x[3]);
	}
}

/* ((a + b) + c) + d */
static void expr4(int x[], struct op *op1, struct op *op2, struct op *op3)
{
	Rational a = { .numerator = x[0],  .denominator = 1};
	Rational b = { .numerator = x[1],  .denominator = 1};
	Rational c = { .numerator = x[2],  .denominator = 1};
	Rational d = { .numerator = x[3],  .denominator = 1};
	Rational t1, t2, res;

	(*op1->f)(&a, &b, &t1);
	(*op2->f)(&t1, &c, &t2);
	(*op3->f)(&t2, &d, &res);

	if (equ(&res, 24)) {
		printf("((%d %c %d) %c %d) %c %d = 24\n",
			x[0], op1->op, x[1], op2->op, x[2], op3->op, x[3]);
	}
}

/* (a + (b + c)) + d */
static void expr5(int x[], struct op *op1, struct op *op2, struct op *op3)
{
	Rational a = { .numerator = x[0],  .denominator = 1};
	Rational b = { .numerator = x[1],  .denominator = 1};
	Rational c = { .numerator = x[2],  .denominator = 1};
	Rational d = { .numerator = x[3],  .denominator = 1};
	Rational v, w, res;

	(*op2->f)(&b, &c, &v);
	(*op1->f)(&a, &v, &w);
	(*op3->f)(&w, &d, &res);

	if (equ(&res, 24)) {
		printf("(%d %c (%d %c %d)) %c %d = 24\n",
			x[0], op1->op, x[1], op2->op, x[2], op3->op, x[3]);
	}
}

void calc24(int x[])
{
	int	i, j, k;

	for (i = 0; i < DIM(ops); ++i) {
		for (j = 0; j < DIM(ops); ++j) {
			for (k = 0; k < DIM(ops); ++k) {
				expr1(x, &ops[i], &ops[j], &ops[k]);
				expr2(x, &ops[i], &ops[j], &ops[k]);
				expr3(x, &ops[i], &ops[j], &ops[k]);
				expr4(x, &ops[i], &ops[j], &ops[k]);
				expr5(x, &ops[i], &ops[j], &ops[k]);
			}
		}
	}
}

static void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void perm(int x[], int a, int b)
{
	int	i;

	if (b - 1 != a) {
		for (i = a; i < b; ++i) {
			swap(&x[a], &x[i]);
			perm(x, a + 1, b);
			swap(&x[a], &x[i]);
		}
	} else {
		calc24(x);
	}
}

int main(void)
{
	int	x[N];
	int	i;

	for (i = 0; i < N; ++i)
		scanf("%d", &x[i]);

	perm(x, 0, N);

	return 0;
}

