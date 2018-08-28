class TempRange {
	int _low;
	int _high;
public:
	int getLow() const {
		return _low;
	}

	int getHigh() const {
		return _high;
	}
};

class HeatingPlan {
	TempRange _range;
public:
	bool withinRange(int low, int high) {
		return _range.getLow() <= low && high <= _range.getHigh();
	}
};

extern TempRange &daysTempRange(void);

class Room {
public:
	bool withinPlan(HeatingPlan &plan) {
		int low = daysTempRange().getLow();
		int high = daysTempRange().getHigh();
		return plan.withinRange(low, high);
	}
};

