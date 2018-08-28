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
public:
	JobItem(int unitPrice, int quantity):
		_unitPrice(unitPrice),
		_quantity(quantity) {
	}

	int getTotalPrice() const {
		return getUnitPrice() * _quantity;
	}

	virtual int getUnitPrice() const {
		return _unitPrice;
	}
	
	int getQuantity() const {
		return _quantity;
	}
};

class LaborItem: public JobItem {
	Employee *_employee;
public:
	LaborItem(int quantity, Employee *employee):
		JobItem(0, quantity),
		_employee(employee) {
		}

	virtual int getUnitPrice() const {
		return _employee->getRate();
	}

	Employee *getEmployee() const {
		return _employee;
	}
};