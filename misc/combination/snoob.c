typedef unsigned long long	UINT64;

UINT64 getNextLarger(UINT64 x)
{
	UINT64	rightOne = x & -x;
	UINT64	nextHigherOneBit = x + rightOne;
	UINT64	rightOnesPattern = x ^ nextHigherOneBit;

	rightOnesPattern /= rightOne;
	rightOnesPattern >>= 2;

	return nextHigherOneBit | rightOnesPattern;
}

