## Next Power of Two

Write a function that, for a given number _n_, finds a number _p_ which is greater than or equal to _n_ and is a power of 2.

```
unsigned int roundUpToNextPowerOfTwo(unsigned int x)
{
    x--;
    x |= x >> 1;  // handle  2 bit numbers
    x |= x >> 2;  // handle  4 bit numbers
    x |= x >> 4;  // handle  8 bit numbers
    x |= x >> 8;  // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x++;
 
    return x;
}
```

### __builtin_clz

### X86 Assembler

```
unsigned int NextPow2(unsigned int Number)
{
    unsigned int RetVal = 1;
    __asm    
    {    
        xor ecx, ecx
        bsr ecx, Number
        inc ecx
        shl RetVal, cl
    }
    return(RetVal);
}
```
