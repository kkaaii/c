class Employee;

class EmployeeType {
public:
	enum {ENGINEER = 0, SALESMAN = 1, MANAGER = 2};

	virtual ~EmployeeType() {}
	virtual int getTypeCode() const = 0;

	virtual int payAmount(Employee &employee) = 0;

	static EmployeeType *newType(int code);
};

class Employee {
	EmployeeType *_type;
	int _monthlySalary;
	int _commission;
	int _bonus;
public:
	Employee(int type): _type(0) {
		setType(type);
	}

public:
	int getType() const {
		return _type->getTypeCode();
	}

	int payAmount() {
		return _type->payAmount(*this);
	}

	void setType(int code) {
		if (0 != _type) {
			delete _type;
			_type = 0;
		}

		_type = EmployeeType::newType(code);
	}

	int getMonthlySalary() const {
		return _monthlySalary;
	}

	int getCommission() const {
		return _commission;
	}

	int getBonus() const {
		return _bonus;
	}
};

class Engineer: public EmployeeType {
public:
	virtual int getTypeCode() const {
		return EmployeeType::ENGINEER;
	}

	virtual int payAmount(Employee &employee) {
		return employee.getMonthlySalary();
	}
};

class Salesman: public EmployeeType {
public:
	virtual int getTypeCode() const {
		return EmployeeType::SALESMAN;
	}

	virtual int payAmount(Employee &employee) {
		return employee.getMonthlySalary() + employee.getCommission();
	}
};

class Manager: public EmployeeType {
public:
	virtual int getTypeCode() const {
		return EmployeeType::MANAGER;
	}

	virtual int payAmount(Employee &employee) {
		return employee.getMonthlySalary() + employee.getBonus();
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

