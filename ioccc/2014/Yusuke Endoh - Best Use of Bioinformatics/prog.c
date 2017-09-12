#include <stdio.h>

#define TA	q=
#define CG	r=

#define Y(z)	d;if (c++%2<1){x=x*4+z;if (c%8>5 && x)putchar(x);}d=

#define A	Y(0)
#define C	Y(1)
#define G	Y(2)
#define T	Y(3)

#define AT	int m(void){d=
#define GC	d; return 0;} int(*f)(void)=m;

void p(int n, int c)
{
    while (n--)
        putchar(c);
}

/*
 * print spaces
 */
int b(int c)
{
    int q;
    int r;

    q = c & 7;
    q = (1 + 7 * q - q * q) / 3;
    c &= 15;
    r = (3 - c) * (c - 12);
    if (r < 0) r = 0;
    p(!q + 4 + r / 6, ' ');

    return q;
}

int(*f)(void),d,c,x,q,r;

int main()
{
    if (f)
        f();
    else {
        puts("#include \"prog.c\"\n"
             "\n"
             "     AT");
        while ((x = getchar()) != EOF) {
            for (d = 64; d;) {
                q = b(++c);
                if (0 == q) {
                    puts(c % 8 ? "CG" : "TA");
                } else {
                    putchar("ACGT"[x/d&3]);
                    p(q,'~');
                    putchar("TGCA"[x/d&3]);
                    putchar('\n');
                    d /= 4;
                }
            }
        }
        b(++c);
        puts("GC");
    }
    return 0;
}

