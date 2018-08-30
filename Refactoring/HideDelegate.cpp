#include <string>

typedef std::string String;

class Person;

class Department {
	Person *_manager;
public:
	Department(Person *manager): _manager(manager) {}

	Person *getManager() const {
		return _manager;
	}
};

class Person {
	Department *_department;
public:
	Person *getManager() const {
		return _department->getManager();
	}
};

void foo()
{
	Person *john;
	Person *manager = john->getManager();
}