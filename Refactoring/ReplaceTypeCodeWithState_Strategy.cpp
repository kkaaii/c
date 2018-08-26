class EmployeeType {
public:
	enum {ENGINEER = 0, SALESMAN = 1, MANAGER = 2};

	virtual ~EmployeeType() {}
	virtual int getTypeCode() const = 0;

	static EmployeeType *newType(int code);
};

class Employee {
	EmployeeType *_type;
public:
	Employee(int type): _type(0) {
		setType(type);
	}

public:
	int getType() const {
		return _type->getTypeCode();
	}

	void setType(int code) {
		if (0 != _type) {
			delete _type;
			_type = 0;
		}

		_type = EmployeeType::newType(code);
	}
};

class Engineer: public EmployeeType {
public:
	virtual int getTypeCode() const {
		return EmployeeType::ENGINEER;
	}
};

class Salesman: public EmployeeType {
public:
	virtual int getTypeCode() const {
		return EmployeeType::SALESMAN;
	}
};

class Manager: public EmployeeType {
public:
	virtual int getTypeCode() const {
		return EmployeeType::MANAGER;
	}
};

EmployeeType *EmployeeType::newType(int code)
{
	switch (code) {
	case ENGINEER:
		return new Engineer();

	case SALESMAN:
		return new Salesman();

	case MANAGER:
		return new Manager();

	default:
		break;
	}

	return 0;
}

