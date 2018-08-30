class Date {
public:
	Date(int year, int month, int date);
	int getYear() const;
	int getMonth() const;
	int getDate() const;
	
	static Date *nextDay(const Date &date) {
		return new Date(date.getYear(), date.getMonth(), date.getDate() + 1);
	}
};

void foo(Date &previousEnd)
{
	Date *newStart = Date::nextDay(previousEnd);
}