#include <string>

typedef std::string String;

class Date {
public:
	Date(String dateString);
	Date(int year, int month, int date);
	int getYear() const;
	int getMonth() const;
	int getDate() const;
};

class DateSub: public Date {
public:
	DateSub(String dateString): Date(dateString) {}
	DateSub(const Date &date): Date(date) {}
	
	Date *nextDay() {
		return new Date(getYear(), getMonth(), getDate() + 1);
	}
};

class DateWrap {
	Date _date;
public:
	DateWrap(String dateString): _date(dateString) {}
	DateWrap(const Date &date): _date(date) {}
	int getYear() const {
		return _date.getYear();
	}
	int getMonth() const {
		return _date.getMonth();
	}
	int getDate() const {
		return _date.getDate();
	}
	Date *nextDay() {
		return new Date(getYear(), getMonth(), getDate() + 1);
	}
};