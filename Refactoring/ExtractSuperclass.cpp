#include <string>
#include <vector>

typedef std::string String;

class Party {
	String _name;
public:
	String getName() const {
		return _name;
	}

	virtual int getAnnualCost() const = 0;

protected:
	Party(String name): _name(name) {}
};

class Employee: public Party {
	String _id;
	int _annualCost;
public:
	Employee(String name, String id, int annualCost):
		Party(name), _id(id), _annualCost(annualCost) {
		}
	
	virtual int getAnnualCost() const {
		return _annualCost;
	}
	
	String getId() const {
		return _id;
	}
};

class Department: public Party {
	typedef std::vector<Employee> Employees;
	typedef Employees::const_iterator Iterator;
	Employees _staff;
public:
	Department(String name): Party(name) {}
	
	virtual int getAnnualCost() const {
		int result = 0;
		for (Iterator it = _staff.begin(); it != _staff.end(); ++it) {
			result += it->getAnnualCost();
		}
		return result;
	}

	int getHeadCount() const {
		return _staff.size();
	}
	
	int addEmployee(const Employee& employee) {
		_staff.push_back(employee);
	}
};