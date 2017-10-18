/******************************************************************************
** File Name: knapsack.c
** Author:
** Creation Time: Tue Oct 17 05:24:55 2017
**
**
** 0-1 knapsack problem
**
** Given a set of n items numbered from 1 up to n, each with a weight w[i]
** and value v[i], along with a maximum weight capacity W,
**
**     maximize sigma(v[i] * x[i])
**     subject to sigma(w[i] * x[i]) <= W and x[i] belongs to {0, 1}
**
** Here x[i] represents the number of instances of item i to include in the
** knapsack. Informally, the problem is to maximize the sum of the values of
** the items in the knapsack so that the sum of the weights is less than or
** equal to the knapsack's capacity.
**
**
** Bounded Knapsack Problem (BKP)
**
** The BKP removes the restriction that there's only one of each item, but
** restricts the number x[i] of copies of each kind of item to a maximum
** non-negative integer value c:
**
**     maximize sigma(v[i] * x[i])
**     subject to sigma(w[i] * x[i]) <= W and 0 <= x[i] <= c
**
**
** Unbounded Knapsack Problem (UKP)
**
** The UKP places no upper bound on the number of copies of each kind of item
** and can be formulated as above except for that the only restriction on x[i]
** is that it is a non-negative integer.
**
**    maximize sigma(v[i] * x[i])
**    subject to sigma(w[i] * x[i]) <= W and x[i] >= 0
*/

/*
** f[i][W] = max{f[i-1][W-k*w[i]] + k*v[i] | 0 <= k*c[i] <= W}
*/
void ukp(unsigned f[], unsigned c, unsigned wi, unsigned vi)
{
	int w;

	for (w = wi; w <= c; ++w) {
		int temp = f[w - wi] + vi;
		if (f[w] < temp)
			f[w] = temp;
	}
}

/*
** f[i][W] = max{f[i-1][W], f[i-1][W-w[i]] + v[i]}
*/
void kp(unsigned f[], unsigned c, unsigned wi, unsigned vi)
{
	for (; c >= wi; --c) {
		int temp = f[c - wi] + vi;
		if (f[c] < temp)
			f[c] = temp;
	}
}

/*
** f[i][W] = max{f[i-1][W-k*w[i]] + k*v[i] | 0 <= k <= x[i]}
*/
void bkp(unsigned f[], unsigned c, unsigned wi, unsigned vi, unsigned xi)
{
	int k;

	if (wi * xi >= c) {
		ukp(f, c, wi, vi);
	} else {
		for (k = 1; k < xi; k <<= 1) {
			kp(f, c, wi * k, vi * k);
			xi -= k;
		}
		if (0 != xi)
			kp(f, c, wi * xi, vi * xi);
	}
}

