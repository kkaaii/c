#include <string>
#include <vector>

typedef std::string String;

class Employee {
	String _name;
	String _id;
	int _annualCost;
public:
	Employee(String name, String id, int annualCost):
		_name(name), _id(id), _annualCost(annualCost) {
		}
	
	int getAnnualCost() const {
		return _annualCost;
	}
	
	String getId() const {
		return _id;
	}
	
	String getName() const {
		return _name;
	}
};

class Department {
	typedef std::vector<Employee> Employees;
	typedef Employees::const_iterator Iterator;
	String _name;
	Employees _staff;
public:
	Department(String name): _name(name) {}
	
	int getTotalAnnualCost() const {
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
	
	String getName() const {
		return _name;
	}
};