/******************************************************************************
** File Name: 1014.c
** Author:
** Creation Time: Tue Oct 17 05:24:55 2017
*/
#include <stdio.h>
#include <string.h>

#define	M	6
#define	N	20000

int marbles[M + 1];

static int input(void)
{
	int valid = 0;
	int v;

	for (v = 1; v <= M; ++v) {
		scanf("%d", &marbles[v]);
		if (0 != marbles[v])
			valid = 1;
	}

	return valid;
}

static int dp(void);

int main(void)
{
	int i;

	for (i = 1; input(); ++i) {
		printf("Collection #%d:\n", i);
		puts(dp() ? "Can be divided.\n" : "Can't be divided.\n");
	}

	return 0;
}

int f[N * M + 1];

int dp(void)
{
	unsigned v;
	unsigned c;

	c = 0;
	for (v = 1; v <= M; ++v) {
		c += v * marbles[v];
	}

	if (c & 1) {
		return 0;
	}

	memset(f, 0, sizeof (f));
	f[0] = 0;

	c /= 2; 
	for (v = 1; v <= M; ++v) {
		bkp(f, c, v, v, marbles[v]);
	}

	return f[c] == c;
}

