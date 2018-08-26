class Employee {
	int _type;
	enum {ENGINEER = 0, SALESMAN = 1, MANAGER = 2};
public:
	Employee(int type): _type(type) {}
};

