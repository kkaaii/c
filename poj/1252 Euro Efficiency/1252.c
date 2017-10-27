/******************************************************************************
** File Name: 1252.c
** Author:
** Creation Time: Mon Oct 23 08:20:03 2017
**
** Result	Memory	Time	Language	Code Length	Submit Time
** Accepted	436K	0MS	GCC		926B		2017-10-28 00:09:01
*/
#include <stdio.h>
#include <string.h>

#define	M	6	/* number of coin values */
#define	V	100	/* value of coins ranges [1..V] */

#define	Z	2000	/*(V * V) */

int v[M + 1];
int f[Z + 1];

static void dp(void);

int main(void)
{
	int n;
	int i;
	int sum;
	int max;

	scanf("%d", &n);
	while (n--) {
		for (i = 1; i <= M; ++i)
			scanf("%d", &v[i]);
		dp();

		sum = 0;
		max = 0;
		for (i = 1; i <= V; ++i) {
			sum += f[i];
			if (max < f[i])
				max = f[i];
		}

		printf("%d.%02d %d\n", sum / V, sum % V, max);
	}

	return 0;
}

#define	INF	0x7f

void dp(void)
{
	int i;
	int c;	/* capacity */
	int w;	/* weight */
	int t;

	memset(f, INF, sizeof f);
	f[0] = 0;

	for (i = 1; i <= M; ++i) {
		w = v[i];
		for (c = w; c <= Z; ++c) {
			t = f[c - w] + 1;
			if (f[c] > t)
				f[c] = t;
		}
	}

	for (i = 1; i <= M; ++i) {
		w = v[i];
		for (c = Z - w; c > 0; --c) {
			t = f[c + w] + 1;
			if (f[c] > t)
				f[c] = t;
		}
	}
}
