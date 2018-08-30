#include <string>

typedef std::string String;

class Date {
public:
	Date(String dateString);
	int getYear() const;
	int getMonth() const;
	int getDate() const;
};