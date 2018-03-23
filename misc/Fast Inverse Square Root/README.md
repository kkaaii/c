# FAST INVERSE SQUARE ROOT
CHARIS LOMONT

## ABSTRACT
Computing reciprocal square roots is necessary in many applications, such as vector normalization in video games. Often, some loss of precision is acceptable for a large increase in speed. This note examines and improves a fast method found in sourcecode for several online libraries, and provides the ideas to derive similar methods for other functions.

## INTRODUCTION
Reading the math programming forum on www.gamedev.net, I ran across an interesting method to compute an inverse square root. The C code was essentially (my comments):

```
float InvSqrt(float x)
{
float xhalf = 0.5f*x;
int i = *(int*)&x; // get bits for floating value
i = 0x5f3759df - (i>>1); // gives initial guess y0
x = *(float*)&i; // convert bits back to float
x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
return x;
}
```

The interesting part was the constant 0x5f3759df: where did it come from and why does the code work? Some quick testing in Visual C++.NET showed the code above to be roughly 4 times faster than the naive (float)(1.0/sqrt(x)), and the maximum relative error over all floating point numbers was 0.00175228, so the method seems very useful. Three immediate questions were: 1) how does it work, 2) can it be improved, and 3) what bit master designed such an incredible hack?

A quick search on Google for 0x5f3759df returned several hits, the most relevant being a reference to a thread about this code on comp.graphics.algorithms from Jan 9, 2002, and an (incorrect, but close) explanation by D. Eberly. However his explanation is illuminating. Further digging found no correct explanation of this code. It appears in the sourcecode for Quake 3, written by legendary game programmer John Carmack, but someone on the net (I cannot now find the reference) credited it to Gary Tarolli who was at Nvidia. Can anyone confirm authorship? It also appears in the Crystal Space sourcecode, the Titan Engine, and the Fast Code Library, although each seems to derive from Quake 3.

The motivation to try such an algorithm is more clearly explained in Eberly, where he assumes the shift creates a linear interpolation to the inverse square root. Note there are several ways to speed up this code, but this note will not go into further optimizations. There are also faster methods, perhaps table lookup tricks, but most of them have less accuracy than this method.

This note assumes PC architecture (32 bit floats and ints) or similar. In particular the floating point representation is IEEE 754-1985. This C code has been reported to be endian-neutral (supposedly tested it on a Macintosh). I have not verified this. Since the method works on 32 bit numbers it seems (surprisingly) endian-neutral. It is easy to extend the code to other situations and bit sizes (such as type double) using the ideas in this note. Anyway, on to the problems 1), 2), and 3).

## Background
Floating point numbers are stored in the PC as 32 bit numbers;

s|E|M
-|-|-
bit 31|30 ← bits → 23|22 ← bits → 0

where s is a 1 bit sign (1 denotes negative), E is an 8 bit exponent, and M is a 23 bit mantissa. The exponent is biased by 127 to accommodate positive and negative exponents, and the mantissa does not store the leading 1, so think of M as a binary number with the decimal point to the left, thus M is a value in I = [0, 1). The represented value is

x = (−1)<sup>s</sup>(1 + M)2<sup>E−127</sup>

These bits can be viewed as the floating point representation of a real number, or thinking only of bits, as an integer. Thus M will be considered a real number in I or as an integer, depending on context. M as a real number is M as an integer divided by 2<sup>23</sup>.

## THE ALGORITHM
The main idea is Newton approximation, and the magic constant is used to compute a good initial guess. Given a floating point value x > 0, we want to compute 1/sqrt(x). Define f(y) = 1/y<sup>2</sup> − x. Then the value we seek is the positive root of f(x). Newton’s root finding method, given a suitable approximation y<sub>n</sub> to the root, gives a better one y<sub>n+1</sub> using

y<sub>n+1</sub> = y<sub>n</sub> − f(y<sub>n</sub>) / f'(y<sub>n</sub>), n ≥ 0

For the f(y) given, this simplifies to y<sub>n+1</sub> = 0.5y<sub>n</sub>(3 − xy<sup>2</sup><sub>n</sub>) which corresponds to the line of code x = x*(1.5f-xhalf*x*x) , where x is
the initial guess, which hereafter will be called y0.
The line of code i = 0x5f3759df - (i>>1) computes this initial
guess y0, roughly by multiplying the exponent for x by −
1
2
, and then
picking bits to minimize error. i>>1 is the shift right operator from
C, which shifts the bits of an integer right one place, dropping the
least significant bit, effectively dividing by 2. Eberly’s explanation was
that this produced linear approximation, but is incorrect; we’ll see the
guess is piecewise linear, and the function being approximated is not
the same in all cases. However I would guess his explanation was the
inspiration for creating this algorithm.
4. The Magic Number(s)
Thus we are left with finding an initial guess. Suppose we are given
a floating point value x > 0, corresponding to the 32 bits
0 E M
as above. Let the exponent e = E −127. Then x = (1 +M)2e
, and the
desired value y = √
1
x = √
1
1+M
2
−e/2
, treating e and M as real numbers,
NOT as integers. For the general case we take a magic constant R,
and analyze y0 = R−(i>>1), where the subtraction is as integers, i is
the bit representation of x, but we view y0 as a real number. R is the
integer representing a floating point value with bits
0 R1 R2
i.e., R1 in the exponent field and R2 in the mantissa field. When we
shift i right one bit, we may shift an exponent bit into the mantissa
field, so we will analyze two cases.
For the rest of this note, for a real number α let bαc denote the
integer less than or equal to α.
