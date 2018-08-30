#include <set>

class Customer;

class Order {
	Customer *_customer;
public:
	Customer *getCustomer() const {
		return _customer;
	}

	void setCustomer(Customer *customer) {
		_customer = customer;
	}
};

typedef std::set<Order *> Orders;

class Customer {
	Orders	_orders;
};