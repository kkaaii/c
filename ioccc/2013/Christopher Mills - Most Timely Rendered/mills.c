#include <math.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>

typedef float A[3];
float D, M[999] = { LT }, *L = NL + M, *P, b, t, *h, *i;

A
  E, Q, U, V, C, c, I =
{
EY};

unsigned char g[2414], *p = g, *e;
int j, k, s, m, n, x, y;

void B(float *d, float *s)
{
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
}

void o(float *d, float *s)
{
    d[0] += s[0];
    d[1] += s[1];
    d[2] += s[2];
}

void a(float *d, float *s)
{
    d[0] -= s[0];
    d[1] -= s[1];
    d[2] -= s[2];
}

float H(float *d, float *s)
{
    return d[0] * s[0] + d[1] * s[1] + d[2] * s[2];
}

void X(float *d, float s)
{
    d[0] *= s;
    d[1] *= s;
    d[2] *= s;
}

void v(float *d, float s)
{
    d[0] += s;
    d[1] += s;
    d[2] += s;
}

float W(float *d)
{
    return sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
}

void r(uint32_t u)
{
    *p++ = u >> 24;
    *p++ = u >> 16;
    *p++ = u >> 8;
    *p++ = u;
}

void O(float * d)
{
  X (d, 1 / W (d));
}

char *z, *f;

void u(char *s)
{
    if (*s) {
        u(s+1);
        *p++ = *s;
    }
}

float G (float x, int p)
{
  for (z = p * 46 + f; 12 ^ *z; x += .5)
    {
      for (D = -4; 5 > D; D++)
	if (9 - *z++)
	  {
	    *P++ = x;
	    *P++ = D / 2;
	    *P++ = 0;
	  }
    }
  return p;
}

 float *

d ()
{
  h = 0;
  D = 1e9;
  a (E, Q);
  O (E);
  2[E] && 0 > (t = (1 + 2[Q]) / 2[E]) && (D = -t, h = P);
  for (i = M; P > i; i += 3)
    {
      B (C, i);
      a (C, Q);
      b = H (E, C);
      (t = b * b - H (C, C) + (i < L ? 99
			       : .6)) >= 0 && 0 < (t = b < t ? t + b : b - t)
	&& D > t && (D = t, h = i);
    }
  return h;
}

void
Y (int N)
{
  float *h, *i;
  A p, n;
  if (!(h = d ()))
    *c = 1[c] = (2[c] = 2[E] / 2) / 2;
  else if (h < L)
    {
      X (c, 0);
      v (c, 1);
    }
  else
    {
      B (p, E);
      X (p, D);
      o (p, Q);
      if (h - P)
	{
	  B (n, p);
	  a (n, h);
	  O (n);
	}
      else
	{
	  X (n, 0);
	  ++2[n];
	}
      B (Q, n);
      X (Q, 1e-4);
      o (p, Q);
      X (c, 0);
      if (N < 8)
	{
	  B (Q, n);
	  X (Q, 2 * H (n, E));
	  a (E, Q);
	  o (E, p);
	  B (Q, p);
	  Y (1 + N);
	  X (c, h - P ? .8 : .2);
	}
      for (i = M; L > i; ++i)
	{
	  B (E, i);
	  B (Q, p);
        if (d() == i)
	        v (c, (h - P ? .1 : .5) * H (n, E));
	}
      v (c, 0.05);
      h - P || (2[c] *= .3, c[1 & lrint (*p) ^ lrint (1[p]) & 1] *= .2);
    }
}

void
Z (char *s)
{
  write (k, g, p - g);
  p = g;
  p += 4;
  u (s ? s : "TADI");
} void

J ()
{
  uint32_t c = ~0;
  e = p;
  p = g;
  r (e - p - 8);
  while (p != e)
    {
      c ^= *p++;
      for (j = 0; 8 > j; j++)
	c = c / 2 ^ c % 2 * 3988292384;
    }
  r (~c);
  write (k, g, p - g);
  p = g;
}

void
q (int c)
{
    *p++ = c;
  m += c;
  m %= c = 65521;
  n += m;
  n %= c;
} void

T (float c)
{
  c = .5 + 255 * c;
  q (0 > c ? 0 : c > 255 ? 255 : c);
} struct sockaddr_in R;

int main ()
{
    time_t i;
    struct tm *b;
    R.win_port = 8224;
    s = socket (R.sin_family = AF_INET, SOCK_STREAM, 0);
    bind (s, (void *) &R, sizeof R);
    listen (s, 1);
    for (;;) {
        k = accept (s, 0, 0);
        for (;;) {
	        ++j;
	        read (k, p, 1);
	        if (*p == '\n') {
	            if (3 > j)
		            break;
	            j = 0;
	        }
	    }
        m = 1;
        write(k, "HTTP/1.0 200 OK\r\n", 17);
        write(k, "Refresh: 1\r\n\r\n", 14);
        write(k, "\211PNG\r\n\032\n", 8); /* signature of PNG */

        p += 4;
        u("RDHI");

        {
            struct ihdr_t {
                uint32_t    width;
                uint32_t    height;
                uint8_t     bitDepth;
                uint8_t     colorType;
                uint8_t     compressionMethod;
                uint8_t     filterMethod;
                uint8_t     interlaceMethod;
            } __attribute__((packed)) *ihdr = (struct ihdr_t *)p;

            ihdr->width = htonl(800);
            ihdr->height = htonl(600);
            ihdr->bitDepth = 8;
            ihdr->colorType = 2;
            ihdr->compressionMethod = 0;
            ihdr->filterMethod = 0;
            ihdr->interlaceMethod = 1;

            p += sizeof (*ihdr);
        }
      J ();
      Z (0);
        *p++ = 120;
        *p++ = 1;
      J ();
      i = time (0);
      b = localtime (&i);
      x = b->tm_sec;
      *I = 45 < x ? x - 60 : 15 > x ? x : 30 - x;
      *U = -I[1];
      1[U] = *I;
      *V = 2[I] * *I;
      1[V] = 2[I] * 1[I];
      2[V] = -*I ** I - 1[I] * 1[I];
      O (U);
      O (V);
      X (U, D = W (I) / 1e3);
      X (V, D);
      P = L;
      y = 1 + (11 + b->tm_hour) % 12;
      9 < y && G (-14, y / 10);
      G (-10, y % 10);
      G (-6, 10);
      y = b->tm_min;
      G (-2, y / 10);
      G (2, y % 10);
      G (6, 10);
      G (10, x / 10);
      G (14, x % 10);
      for (z = "xxxdtrb!  d r y "; 9[z]; ++z)
	{
	  for (y = 7 & 8[z]; 600 > y; y += 14 & *z)
	    {
	      Z (0);
            *p++ = 0;
	      p += 4;
	      q (0);
	      for (x = 7 & 9[z]; 800 > x; x += 15 & 1[z])
		{
		  B (Q, V);
		  X (Q, y - 300);
		  B (E, U);
		  X (E, x - 400);
		  o (E, Q);
		  B (Q, I);
		  Y (0);
		  T (*c);
		  T (1[c]);
		  T (2[c]);
		}
	      j = p - g - 13;
	      12[g] = ~(10[g] = j >> 8);
	      11[g] = ~(9[g] = j);
	      J ();
	    }
	}
      Z (0);
        *p++ = 1;
      r (65535);
      r (n << 16 | m);
      n = 0;
      J ();

        write(k, "\0\0\0\0IEND\xAE\x42\x60\x82", 12);
      j = 0;
      close (k);
    }
}

char *f = "	       	 							  							  							 	       	 					 		 						 	          								 								   				 	 		 				  			 			  			 			  				   		 					 	 			 			  			 			  			 			 	   	   					   						 		 					 			          				 					 		      			 			  				 		  				 		 	    			 	      		 		 			 	 			 			  			 			 	   			 									   						 		  				 				  		 						   	   	   	 			 			  			 			  			 			 	   	   						   	 			 			  			 			 	 			 		 		      																					 			 																				";
