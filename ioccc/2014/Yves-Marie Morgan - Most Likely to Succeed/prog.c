
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define A calloc(1, sizeof(m))
#define D m.
#define L ->
#define P malloc(
#define R &&
#define S ++
#define U for(
#define V if(

typedef struct T*_; struct T{ int t,i; char*a; _*b,c,d,e,f; } m,x,y; struct stat z; int u; void * H;
int K(int c){ return strchr(" \n\t",c); } void B(_ s,_ p) { s ->b=realloc(s ->b,S s ->i*sizeof(_)); s ->b [s
->i-1]=p; } _ C(char*d,int l){ _ s=A; s ->a=d?d:P 1); s ->i=l<0?d?strlen(d):0:l; return s; } _ E(_ s){ _ r
=A; int i=0; U; i<s ->i; ){ U; i<s ->i R K(s ->a[i]); )i S; int j=i; U; j<s ->i R !K(s ->a[j]); )j S; V j-i)B(
r,C(s ->a+i,j-i)); i=S j; } return r; } int F(_ a,_ b){ return a ->i==b ->i R !memcmp(a ->a,b ->a,a ->i); } void
I( _    s,char c ) {    char*d=P            1); *d=c    ; B(s,C(d,1)    ); }                    void
 J (    char*d,int l    ,int o,_            v){ _ n=    C(d,l); _ w;    U l=                    0; l
<D d        ->i; )        { w=    D d ->b[l S ];    V F(    w ->c,n)){    V o>w ->i)    return; goto  O; } }
B( D        d,w=        A );    w ->c=n; O:w ->i    =o ;    w L d=v;    } int M(    _ b,int p,char*t,int
g ){    _ f=    A; f    L e=                    A; f            ->a=t; int h=g            ,i=0,j=0,n=b
 ->i ;    char    c=0,    d,*o                    =b ->a            ; U; p<n; ){            t=j?"":f ->a ;
 int    k=p; _ r=H ;    U; p    <n; ){ int l    = 0;    U; p<n R    o[p]==92    ; ){ l S; p S; } c=o
[p S    ]; V c^10||!    l% 2    ||!g){ d=c==    35 R    !i R!g||    c== 10 R    g^2; V d||strchr(t,c
)) {    r=C(o+k,p-k-    d*l/    2-1); V  d R    l%2)    { r ->a[r ->i-1]    =c^35                   ?32:
c; c    =0; } break;    }  }    c=0; } V!j)B    (f L    e,r?r:C(o+k,    n-k)                    ); r
=A; switch(c){ case 35:j S; case 0:break; case 10:goto O; case 36:switch(d=o[p S]){ case 36:I(f L e,
d); break; default:I(r ->c=A,d); goto o; case 40:case 123:r L f=f; r L e=A; r ->a=d^40?"}$":")$"; f=r; }
break; default:V f L f){ r ->c=f L e; f=f L f; o:r L t S; B(f L e,r); } else goto O; } i=f L f R g==1;
g=i?0:h; } O: x.c=f L e; x.t=c; return p; } _ N(_ s){ _ o=C(H,0); U int i=0; i<s ->i; ){ _ p=s ->b[i S];
V p L t){ _ n=N(p ->c); p=C(H,0); U int j=0; j<D d ->i; ){ _ w=D d ->b[j S]; V F(w ->c,n)){ M(w L d,0,"$",2)
; p=N(x.c); break; } } } o ->a=realloc(o ->a,o ->i+=p ->i); memcpy(o ->a+o ->i-p ->i,p ->a,p ->i); } return o; } _ O(_
t){ U int i=0; i<D e ->i; ){ _ s=D e ->b[i S]; V F(s ->c,t))return s; }_ s=A; s ->c=t; s L d=A; s L t--; B(D
e,s); return s; } void Q(_ s){ _ d=s ->c; V!stat(memcpy(calloc(1,d ->i+1),d ->a,d ->i),&z))s L t=z.st_mtime;
V!s ->i S){ u=!s L d ->i R s L t<0; int i=0,j,k=s L t<0; _ c=H,t; U; !u R i<s L d ->i; ){ _ a=s L d ->b[i S]
; U j=0; !u R j<a L d ->i; ){ Q(t=O(a L d ->b[j S])); k|=t L t<0||t L t>s L t; } V a ->c ->i){ c=a ->c; V a  L
d ->i)J("<", 1, 0, *a L d ->b); } } U j=0; !u R c R k R j<c ->i; ){ J("@",1,0,d); s=N(c ->b[j S]); U; s ->i  R
strchr("@+-",*s ->a); )s ->a S; u=system(s ->a); } } } void G(_ b){ U; b ->i R K(*b ->a); ){ b ->a S; b ->i--; } }

int main(int i,char**a,char**e){ D d=A; D e=A; D c=A; char p[]="NBLF\1Nblfgjmf\1",*q=p; U; (*q++)--;
); J(p,4,0,C(*a S,-1)); U i=0; i<3; i+=2){ U; *a; ){ q=*a; U; *q R*q^61; )q S; V*q)J(*a,q-*a,i+1,C(q
+1,-1)); else V!i)B(D c,C(*a,-1)); a S; } a=e; } stat(p+5,&z); i=z.st_size; _ b=C(H,0),c,d; b ->i=read
(open(p+5,0),b ->a=P i),i); U; u<i; ){ V b ->a[u]^9){ U; u<i R K(b ->a[u]); )u S; u=M(b,u,"=:$",0); y = x;
switch(x.t){ case 58:u=M(b,u,"$",0); D f=c=A; c L d=E(N(x.c)); c ->c=A; d=E(N(y.c)); U; d ->i--; )B(O(*d
->b S) L d,c); break; case 61:u=M(b,u,"",0); G(c=N(x.c)); G(d=N(y.c)); U; d ->i--R K(d ->a[d ->i]); ); J(d ->a
,S d ->i,2,c); } } else { u=M(b,S u,"$",1); I(x.c,0); B(D f ->c,x.c); } } V!D c ->i R D e ->i)B(D c,D e ->b[0]
->c); U u=0; !u R D c ->i--; )Q(O(*D c ->b S)); return u; }
