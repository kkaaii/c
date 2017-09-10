#include <stdio.h>

#define H   50
#define W   80

#define N   255

#define V   8.0

/* <-- Configure here: X1, X2, Y1, Y2 */
#define X1 -2.0
#define X2  1.0
#define Y1 -1.3
#define Y2  1.3

int mandelbrot(double x0, double y0)
{
    int i;
    double xx, x = 0.0;
    double yy, y = 0.0;

    for (i = 1; i < N; ++i) {
        xx = x * x;
        yy = y * y;
        if (xx + yy >= V)
            break;
        y  = y0 + 2 * x * y;
        x  = x0 + (xx - yy);
    } 

    return i;
}

int main()
{
    int ix, iy;

    for (iy = 0; iy <= H; ++iy) {
        for (ix = 0; ix <= W; ++ix) {
            double x0 = ix * (X2 - X1) / W + X1;
            double y0 = iy * (Y2 - Y1) / H + Y1;
            putchar(" +@-.*"[mandelbrot(x0, y0) % 5]);
        }
        putchar('\n');
    }

    return 0;
}

