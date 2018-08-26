class Employee {
public:
	enum {ENGINEER = 0, SALESMAN = 1, MANAGER = 2};
	static Employee *create(int type);
	virtual ~Employee() {}
	virtual int getType() const = 0;
};

class Engineer: public Employee {
public:
	virtual int getType(void) const {
		return Employee::ENGINEER;
	}
};

class Salesman: public Employee {
public:
	virtual int getType(void) const {
		return Employee::SALESMAN;
	}
};

class Manager: public Employee {
public:
	virtual int getType(void) const {
		return Employee::MANAGER;
	}
};

Employee *Employee::create(int type)
{
	Employee *employee = 0;

	switch (type) {
	case Employee::ENGINEER:
		employee = new Engineer();
		break;

	case Employee::SALESMAN:
		employee = new Salesman();
		break;

	case Employee::MANAGER:
		employee = new Manager();
		break;

	default:
		break;
	}

	return employee;
}

