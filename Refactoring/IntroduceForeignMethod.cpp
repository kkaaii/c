class Date {
public:
	Date(int year, int month, int date);
	int getYear() const;
	int getMonth() const;
	int getDate() const;
};

void foo(Date &previousEnd)
{
	Date newStart(previousEnd.getYear(),
		previousEnd.getMonth(),
		previousEnd.getDate() + 1);
}