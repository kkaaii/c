#include <string>

typedef std::string String;

class TelephoneNumber {
	String _areaCode;
	String _number;
public:
	String getTelephoneNumber() const {
		return "(" + _areaCode + ")" + _number;
	}
	
	String getAreaCode() const {
		return _areaCode;
	}
	
	String getNumber() const {
		return _number;
	}
};

class Person {
	String _name;
	TelephoneNumber &_officeTelephone;
public:
	String getName() const {
		return _name;
	}
	
	String getTelephoneNumber() const {
		return "(" + getOfficeAreaCode() + ")" + getOfficeNumber();
	}
	
	String getOfficeAreaCode() const {
		return _officeTelephone.getAreaCode();
	}
	
	String getOfficeNumber() const {
		return _officeTelephone.getNumber();
	}
};