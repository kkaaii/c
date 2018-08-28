class Employee {
	int _type;

public:
	enum {ENGINEER = 0, SALESMAN = 1, MANAGER = 2};
	Employee(int type): _type(type) {}

	static Employee *create(int type);
};

class Engineer: public Employee {
public:
	Engineer(): Employee(ENGINEER) {}
};

class Salesman: public Employee {
public:
	Salesman(): Employee(SALESMAN) {}
};

class Manager: public Employee {
public:
	Manager(): Employee(MANAGER) {}
};


Employee *Employee::create(int type)
{
	switch (type) {
	case ENGINEER:
		return new Engineer();

	case SALESMAN:
		return new Salesman();

	case MANAGER:
		return new Manager();
	}
}

int main()
{
	Employee *kent = Employee::create(Employee::ENGINEER);
}

