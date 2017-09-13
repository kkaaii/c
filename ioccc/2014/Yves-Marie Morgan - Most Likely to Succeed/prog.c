
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct T*_;
struct T {
    int t,i;
    char*a; 
    _ *b,c,d,e,f;
} m,x,y;

struct stat z;
int u;
void * H;

int isspace(int c)
{
    return strchr(" \n\t",c);
}

void addchild(_ s, _ p)
{
    s->b = realloc(s->b, ++s->i * sizeof(_));
    s->b[s->i - 1] = p;
}

_ newnode(char *d, int l)
{
    _ s = calloc(1, sizeof(m));
    s->a = d ? d : malloc(1);
    s->i = l < 0 ? d ? strlen(d) : 0 : l;
    return s;
}

_ E(_ s)
{
    _ r = calloc(1, sizeof(m));
    int i = 0;

    for (; i < s->i; ) {
        for (; i < s->i && isspace(s->a[i]); )
            i++;

        int j = i;
        for (; j < s->i && !isspace(s ->a[j]); )
            j++;

        if (j - i)
            addchild(r, newnode(s->a + i, j - i));
        i = ++j;
    }
    return r;
}

int equal(_ a, _ b)
{
    return a->i == b->i && !memcmp(a->a, b->a, a->i);
}

void I(_ s, char c)
{
    char *d = malloc(1);
    *d = c;
    addchild(s, newnode(d, 1));
}

void J(char *d, int l, int o, _ v)
{
    _ n = newnode(d, l);
    _ w;

    for (l = 0; l < m.d->i; ) {
        w = m.d->b[l++];

        if (equal(w->c, n)) {
            if (o > w->i)
                return;
            goto O;
        }
    }

    addchild(m.d, w = calloc(1, sizeof(m)));
    w->c = n;
O:
    w->i = o;
    w->d = v;
}

int M(_ b, int p, char *t, int g) {
    _ f = calloc(1, sizeof(m));
    f->e = calloc(1, sizeof(m));
    f->a = t;

    int h = g, i = 0, j = 0, n = b->i;
    char c = 0, d, *o = b->a;

    for (; p < n; ) {
        t = j ? "" : f->a;
        int k = p;
        _ r = H;
        for (; p < n; ) {
            int l = 0;
            for (; p < n && o[p] == 92; ) {
                l++;
                p++;
            }
            c = o[p++];
            if (c != '\n' || !l %  2 || !g) {
                d = c == '#' && !i && !g || c == '\n' && g ^ 2;
                if (d || strchr(t, c)) {
                    r = newnode(o + k, p - k - d * l / 2 - 1);
                    if (d && l % 2) {
                        r->a[r->i - 1] = c != '#' ? ' ' : c;
                        c = '\0';
                    }
                    break;
                }
            }
            c = '\0';
        }

        if (!j)
            addchild(f->e, r ? r : newnode(o + k, n - k));
        r = calloc(1, sizeof(m));
        switch(c) {
        case '#':
            j++;
        case '\0':
            break;
        case '\n':
            goto O;
        case '$':
            switch(d = o[p++]) {
            case '$':
                I(f->e, d);
                break;
            default:
                I(r->c = calloc(1, sizeof(m)),d);
                goto o;
            case '(':
            case '{':
                r->f = f;
                r->e = calloc(1, sizeof(m));
                r->a = d != '(' ? "}$" : ")$";
                f = r;
            }
            break;
        default:
            if (f->f) {
                r->c = f->e;
                f = f->f;
o:
                r->t++;
                addchild(f->e, r);
            } else
                goto O;
        }
        i = f->f && g == 1;
        g = i ? 0 : h;
    }
O:
    x.c = f->e;
    x.t = c;
    return p;
}

_ N(_ s)
{
    _ o = newnode(H, 0);
    for (int i = 0; i < s->i; ) {
        _ p = s->b[i++];
        if (p-> t) {
            _ n = N(p->c);
            p = newnode(H, 0);
            for (int j = 0; j < m.d->i; ) {
                _ w = m.d->b[j++];
                if (equal(w->c, n)) {
                    M(w->d, 0, "$", 2);
                    p = N(x.c);
                    break;
                }
            }
        }
        o->a = realloc(o->a, o->i += p->i);
        memcpy(o->a + o->i - p->i, p->a, p->i);
    }
    return o;
}

_ O(_ t)
{
    for (int i = 0; i < m.e->i; ) {
        _ s = m.e->b[i++];
        if (equal(s->c, t))
            return s;
    }
    _ s = calloc(1, sizeof(m));
    s->c = t;
    s->d = calloc(1, sizeof(m));
    s->t--;
    addchild(m.e, s);
    return s;
}

void Q(_ s)
{
    _ d = s->c;
    if (!stat(memcpy(calloc(1, d->i + 1), d->a, d->i), &z))
        s->t = z.st_mtime;
    if (!s->i++) {
        u = !s->d->i && s->t < 0;
        int i = 0, j, k = s->t < 0;
        _ c = H, t;
        for (; !u && i < s->d->i; ) {
            _ a = s->d->b[i++];
            for (j = 0; !u && j < a->d->i; ) {
                Q(t = O(a->d->b[j++]));
                k |= t->t < 0 || t->t > s->t;
            }
            if (a->c->i) {
                c = a->c;
                if (a->d->i)
                    J("<", 1, 0, *a->d->b);
            }
        }
        for (j = 0; !u && c && k && j < c->i; ) {
            J("@", 1, 0, d);
            s = N(c->b[j++]);
            for (; s->i && strchr("@+-", *s->a); )
                s->a++;
            u = system(s->a);
        }
    }
}

void trim(_ b)
{
    for (; b->i && isspace(*b->a); ) {
        b->a++;
        b->i--;
    }
}

int main(int i, char**argv, char**e)
{
    m.d = calloc(1, sizeof(m));
    m.e = calloc(1, sizeof(m));
    m.c = calloc(1, sizeof(m));

    char p[] = "MAKE\0Makefile\0", *q = p;

    J(p, 4, 0, newnode(*argv++, -1));
    for (i = 0; i < 3; i += 2) {
        for (; *argv; ) {
            q = *argv;
            for (; *q && *q != '='; )
                q++;
            if (*q)
                J(*argv, q - *argv, i + 1, newnode(q + 1, -1));
            else if (!i)
                addchild(m.c, newnode(*argv, -1));
            argv++;
        }
        argv = e;
    }
    stat(p + 5, &z);
    i = z.st_size;
    _ b = newnode(H, 0), c, d;
    b->i = read(open(p + 5, 0), b->a = malloc(i), i);
    for (; u < i; ) {
        if (b->a[u] != '\t') {
            for (; u < i && isspace(b->a[u]); )
                u++;
            u = M(b, u, "=:$", 0);
            y = x;
            switch(x.t) {
            case ':':
                u = M(b, u, "$", 0);
                m.f = c = calloc(1, sizeof(m));
                c->d = E(N(x.c));
                c->c = calloc(1, sizeof(m));
                d = E(N(y.c));
                for (; d->i--; )
                    addchild(O(*d->b++)->d, c);
                break;
            case '=':
                u = M(b, u, "", 0);
                trim(c = N(x.c));
                trim(d = N(y.c));
                for (; d->i-- && isspace(d->a[d->i]); )
                    ;
                J(d->a, ++d->i, 2, c);
            }
        } else {
            u = M(b, ++u, "$", 1);
            I(x.c, 0);
            addchild(m.f->c, x.c);
        }
    }
    if (!m.c->i && m.e->i)
        addchild(m.c, m.e->b[0]->c);
    for (u = 0; !u && m.c->i--; )
        Q(O(*m.c->b++));
    return u;
}
