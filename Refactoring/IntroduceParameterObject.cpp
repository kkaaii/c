#include <vector>

class Date {};

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
	double getFlowBetween(Date start, Date end) {
		double result = 0.0;
		for (Iterator it = _entries.begin(); it != _entries.end(); ++it) {
			if (it->getDate() >= start) || it->getDate() <= end) {
				result += it->getValue();
			}
		}
		
		return result;
	}
};
