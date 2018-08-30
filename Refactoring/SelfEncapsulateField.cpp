class IntRange {
	int _lower;
	int _upper;
public:
	IntRange(int lower, int upper): _lower(lower), _upper(upper) {}
	bool includes(int value) const {
		return getLower() <= value && value <= getUpper();
	}
	
	int getLower() const {
		return _lower;
	}
	
	int getUpper() const {
		return _upper;
	}
};

class CappedRange: public IntRange {
	int _capped;
public:
	CappedRange(int lower, int upper, int capped):
		IntRange(lower, upper), _capped(capped) {}

	int getCap() const {
		return _capped;
	}
	
	int getUpper() const {
		return min(IntRange::getUpper(), getCap());
	}
};