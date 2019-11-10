#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#define N_MIN   3
#define N_MAX   100

#define EPSILON 1e-10

int n;
double a[1 + N_MAX];
double a_max;

void dp(double d_min, double d_max);

int main()
{
    int i;
    int pos;
    double sum;

    while (~scanf("%d", &n)) {
        pos = 0;
        sum = 0.0;
        for (i = 0; i < n; ++i) {
            scanf("%lf", &a[i]);
            sum += a[i];
            if (a[i] > a[pos]) pos = i;
        }
        a_max = a[pos];

        dp(a_max, sum);
    }

    return 0;
}

double radian(double d)
{
    int i;
    double sum = 0;

    for (i = 0; i < n; ++i) {
        sum += asin(a[i] / d);
    }

    return sum;
}

double area(double d)
{
    int i;
    double sum = 0;

    for (i = 0; i < n; ++i) {
        sum += a[i] * sqrt(d * d - a[i] * a[i]) / 4;
    }

    return sum;
}

void dp(double d_min, double d_max)
{
    int flag = (radian(d_min) < M_PI);
    double d;
    double sum;

    if (d_max < d_min * 2) {
        puts("0.00");
        return;
    }

    while (d_min + EPSILON < d_max) {
        d = (d_min + d_max) / 2;
        sum = radian(d);
        if (flag) {
            sum = sum + M_PI - 2 * asin(a_max / d);
        }

        if (flag != (sum < M_PI)) {
            d_max = d;
        } else {
            d_min = d;
        }
    }

    sum = area((d_min + d_max) / 2);
    if (flag) {
        sum -= a_max * sqrt(d * d - a_max * a_max) / 2;
    }

    printf("%.2f\n", sum);
}
