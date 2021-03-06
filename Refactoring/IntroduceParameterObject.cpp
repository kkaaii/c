#include <vector>

class Date {};

class DateRange {
	const Date _start;
	const Date _end;
public:
	DateRange(Date start, Date end): _start(start), _end(end) {}

	bool includes(Date date) {
		return _start <= date && date <= _end;
	}
};

class Entry {
	double _value;
	Date _chargeDate;
public:
	Entry(double value, Date chargeDate): _value(value), _chargeDate(chargeDate) {}

	Date getDate() const {
		return _chargeDate;
	}

	double getValue() const {
		return _value;
	}
};

class Account {
	std::vector<Entry> _entries;
	typedef std::vector<Entry>::iterator Iterator;
public:
	double getFlowBetween(DateRange range) {
		double result = 0.0;
		for (Iterator it = _entries.begin(); it != _entries.end(); ++it) {
			if (range.includes(it->getDate())) {
				result += it->getValue();
			}
		}
		
		return result;
	}
};
