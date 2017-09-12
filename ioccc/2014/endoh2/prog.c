#include <stdio.h>

#define TA	q=
#define CG	r=

#define Y(z)	d;if (c++%2<1){x=x*4+z;if (c%8>5 && x)p(1,x);}d=

#define A	Y(0)
#define C	Y(1)
#define G	Y(2)
#define T	Y(3)

#define AT	int m(void){d=
#define GC	d; return 0;} int(*f)(void)=m;

void p(int n, int c)
{
    for (;n--;)
        putchar(c);
}

int(*f)(void),d,c,x,q,r;

int main()
{
    if (f)
        f();
    else {
        puts("#include \"prog.c\"\n\n     AT");
        for (; d = !d ? x = (x = getchar()) < 0 ? 0 : x, 8 * 8 : d,
            q=++c%8,
            q=(1+7*q-q*q)/3,
            r=c*15-c*c-36,
            p(r < 0 ? !q+4 : r/6+!q+4, 32),
            q||x; c%=16)
            if (q) {
                p(1, "ACGT"[x/d&3]);
                p(q,'~');
                p(1, "TGCA"[x/d&3]);
                d/=4;
                p(1,'\n');
            } else {
                puts(c%8?"CG":"TA");
            }
        puts("GC");
    }
    return 0;
}

