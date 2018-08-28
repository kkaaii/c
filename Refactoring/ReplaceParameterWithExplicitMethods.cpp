class Employee {
	int _type;

public:
	enum {ENGINEER = 0, SALESMAN = 1, MANAGER = 2};
	Employee(int type): _type(type) {}

	static Employee *createEngineer() {
		return new Employee(ENGINEER);
	}

	static Employee *createSalesman() {
		return new Employee(SALESMAN);
	}

	static Employee *createManager() {
		return new Employee(MANAGER);
	}
};

int main()
{
	Employee *kent = Employee::createEngineer();
}

