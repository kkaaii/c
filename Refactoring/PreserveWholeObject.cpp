class TempRange {
	int _low;
	int _high;
public:
	bool includes(const TempRange &range) const {
		return _low <= range._low && range._high <= _high;
	}
};

class HeatingPlan {
	TempRange _range;
public:
	bool withinRange(const TempRange &roomRange) const {
		return _range.includes(roomRange);
	}
};

extern TempRange &daysTempRange(void);

class Room {
public:
	bool withinPlan(HeatingPlan &plan) {
		return plan.withinRange(daysTempRange());
	}
};

