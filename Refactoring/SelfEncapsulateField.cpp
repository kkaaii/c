class IntRange {
	int _lower;
	int _upper;
public:
	IntRange(int lower, int upper): _lower(lower), _upper(upper) {}
	bool includes(int value) const {
		return _lower <= value && value <= _upper;
	}
};

class CappedRange: public IntRange {
	int _capped;
public:
	CappedRange(int lower, int upper, int capped):
		IntRange(lower, upper), _capped(capped) {}
};