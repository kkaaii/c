class Employee {
	int _rate;
public:
	Employee(int rate): _rate(rate) {}
	int getRate() const {
		return _rate;
	}
};

class JobItem {
	int	_unitPrice;
	int	_quantity;
	bool	_isLabor;
	Employee *_employee;
public:
	JobItem(int unitPrice, int quantity, bool isLabor, Employee *employee):
		_unitPrice(unitPrice),
		_quantity(quantity),
		_isLabor(isLabor),
		_employee(employee) {
	}
	
	int getTotalPrice() const {
		return getUnitPrice() * _quantity;
	}

	int getUnitPrice() const {
		return _isLabor ? _employee->getRate() : _unitPrice;
	}
	
	int getQuantity() const {
		return _quantity;
	}
	
	Employee *getEmployee() const {
		return _employee;
	}
};
