#include <set>

class Customer;

class Order {
	Customer *_customer;
public:
	Customer *getCustomer() const {
		return _customer;
	}

	void setCustomer(Customer *customer);
};

typedef std::set<Order *> Orders;

class Customer {
	Orders	_orders;
public:
	void add(Order *order) {
		_orders.add(order);
	}

	void remove(Order *order) {
		_orders.remove(order);
	}
};

void Order::setCustomer(Customer *customer)
{
	if (!_customer) _customer.remove(this);
	_customer = customer;
	if (!_customer) _customer.add(this);
}