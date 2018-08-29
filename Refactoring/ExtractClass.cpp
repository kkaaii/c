#include <string>

typedef std::string String;

class Person {
	String _name;
	String _officeAreaCode;
	String _officeNumber;
public:
	String getName() const {
		return _name;
	}
	
	String getTelephoneNumber() const {
		return "(" + _officeAreaCode + ")" + _officeNumber;
	}
	
	String getOfficeAreaCode() const {
		return _officeAreaCode;
	}
	
	String getOfficeNumber() const {
		return _officeNumber;
	}
};