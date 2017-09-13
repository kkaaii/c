
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define A calloc(1, sizeof(m))
#define D m.
#define P malloc(

typedef struct T*_; struct T{ int t,i; char*a; _*b,c,d,e,f; } m,x,y; struct stat z; int u; void * H;
int K(int c){ return strchr(" \n\t",c); } void B(_ s,_ p) { s ->b=realloc(s ->b,++ s ->i*sizeof(_)); s ->b [s
->i-1]=p; } _ C(char*d,int l){ _ s=A; s ->a=d?d:P 1); s ->i=l<0?d?strlen(d):0:l; return s; } _ E(_ s){ _ r
=A; int i=0; for (; i<s ->i; ){ for (; i<s ->i && K(s ->a[i]); )i ++; int j=i; for (; j<s ->i && !K(s ->a[j]); )j ++; if ( j-i)B(
r,C(s ->a+i,j-i)); i=++ j; } return r; } int F(_ a,_ b){ return a ->i==b ->i && !memcmp(a ->a,b ->a,a ->i); } void
I( _    s,char c ) {    char*d=P            1); *d=c    ; B(s,C(d,1)    ); }                    void
 J (    char*d,int l    ,int o,_            v){ _ n=    C(d,l); _ w;    for ( l=                    0; l
<D d        ->i; )        { w=    D d ->b[l ++ ];    if ( F(    w ->c,n)){    if ( o>w ->i)    return; goto  O; } }
B( D        d,w=        A );    w ->c=n; O:w ->i    =o ;    w -> d=v;    } int M(    _ b,int p,char*t,int
g ){    _ f=    A; f    -> e=                    A; f            ->a=t; int h=g            ,i=0,j=0,n=b
 ->i ;    char    c=0,    d,*o                    =b ->a            ; for (; p<n; ){            t=j?"":f ->a ;
 int    k=p; _ r=H ;    for (; p    <n; ){ int l    = 0;    for (; p<n &&    o[p]==92    ; ){ l ++; p ++; } c=o
[p ++    ]; if ( c^10||!    l% 2    ||!g){ d=c==    35 &&    !i &&!g||    c== 10 &&    g^2; if ( d||strchr(t,c
)) {    r=C(o+k,p-k-    d*l/    2-1); if (  d &&    l%2)    { r ->a[r ->i-1]    =c^35                   ?32:
c; c    =0; } break;    }  }    c=0; } if (!j)B    (f ->    e,r?r:C(o+k,    n-k)                    ); r
=A; switch(c){ case 35:j ++; case 0:break; case 10:goto O; case 36:switch(d=o[p ++]){ case 36:I(f -> e,
d); break; default:I(r ->c=A,d); goto o; case 40:case 123:r -> f=f; r -> e=A; r ->a=d^40?"}$":")$"; f=r; }
break; default:if ( f -> f){ r ->c=f -> e; f=f -> f; o:r -> t ++; B(f -> e,r); } else goto O; } i=f -> f && g==1;
g=i?0:h; } O: x.c=f -> e; x.t=c; return p; } _ N(_ s){ _ o=C(H,0); for ( int i=0; i<s ->i; ){ _ p=s ->b[i ++];
if ( p -> t){ _ n=N(p ->c); p=C(H,0); for ( int j=0; j<D d ->i; ){ _ w=D d ->b[j ++]; if ( F(w ->c,n)){ M(w -> d,0,"$",2)
; p=N(x.c); break; } } } o ->a=realloc(o ->a,o ->i+=p ->i); memcpy(o ->a+o ->i-p ->i,p ->a,p ->i); } return o; } _ O(_
t){ for ( int i=0; i<D e ->i; ){ _ s=D e ->b[i ++]; if ( F(s ->c,t))return s; }_ s=A; s ->c=t; s -> d=A; s -> t--; B(D
e,s); return s; } void Q(_ s){ _ d=s ->c; if (!stat(memcpy(calloc(1,d ->i+1),d ->a,d ->i),&z))s -> t=z.st_mtime;
if (!s ->i ++){ u=!s -> d ->i && s -> t<0; int i=0,j,k=s -> t<0; _ c=H,t; for (; !u && i<s -> d ->i; ){ _ a=s -> d ->b[i ++]
; for ( j=0; !u && j<a -> d ->i; ){ Q(t=O(a -> d ->b[j ++])); k|=t -> t<0||t -> t>s -> t; } if ( a ->c ->i){ c=a ->c; if ( a  ->
d ->i)J("<", 1, 0, *a -> d ->b); } } for ( j=0; !u && c && k && j<c ->i; ){ J("@",1,0,d); s=N(c ->b[j ++]); for (; s ->i  &&
strchr("@+-",*s ->a); )s ->a ++; u=system(s ->a); } } } void G(_ b){ for (; b ->i && K(*b ->a); ){ b ->a ++; b ->i--; } }

int main(int i,char**a,char**e){ D d=A; D e=A; D c=A; char p[]="NBLF\1Nblfgjmf\1",*q=p; for (; (*q++)--;
); J(p,4,0,C(*a ++,-1)); for ( i=0; i<3; i+=2){ for (; *a; ){ q=*a; for (; *q &&*q^61; )q ++; if (*q)J(*a,q-*a,i+1,C(q
+1,-1)); else if (!i)B(D c,C(*a,-1)); a ++; } a=e; } stat(p+5,&z); i=z.st_size; _ b=C(H,0),c,d; b ->i=read
(open(p+5,0),b ->a=P i),i); for (; u<i; ){ if ( b ->a[u]^9){ for (; u<i && K(b ->a[u]); )u ++; u=M(b,u,"=:$",0); y = x;
switch(x.t){ case 58:u=M(b,u,"$",0); D f=c=A; c -> d=E(N(x.c)); c ->c=A; d=E(N(y.c)); for (; d ->i--; )B(O(*d
->b ++) -> d,c); break; case 61:u=M(b,u,"",0); G(c=N(x.c)); G(d=N(y.c)); for (; d ->i--&& K(d ->a[d ->i]); ); J(d ->a
,++ d ->i,2,c); } } else { u=M(b,++ u,"$",1); I(x.c,0); B(D f ->c,x.c); } } if (!D c ->i && D e ->i)B(D c,D e ->b[0]
->c); for ( u=0; !u && D c ->i--; )Q(O(*D c ->b ++)); return u; }
