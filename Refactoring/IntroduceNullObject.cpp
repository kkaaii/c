#include <string>

typedef typename std::string String;

class BillingPlan {
};

class PaymentHistory {
public:
	static PaymentHistory *newNull();

	virtual int getWeeksDelinquentInLastYear() const {
		return _weeksDelinquentInLastYear;
	}

private:
	int _weeksDelinquentInLastYear;
};

class NullPaymentHistory: public PaymentHistory {
public:
	virtual int getWeeksDelinquentInLastYear() const {
		return 0;
	}
};

PaymentHistory *PaymentHistory::newNull()
{
	return new NullPaymentHistory();
}

class Customer {
public:
	static Customer *newNull();

	virtual bool isNull() const {
		return false;
	}

	virtual String getName() const;
	virtual BillingPlan getPlan() const;
	virtual PaymentHistory *getHistory() const;
};

class NullCustomer: public Customer {
public:
	virtual bool isNull() const {
		return true;
	}

	virtual String getName() const {
		return "occupant";
	}

	virtual BillingPlan getPlan() const {
		return BillingPlan::basic();
	}

	virtual PaymentHistory *getHistory() const {
		return PaymentHistory::newNull();
	}
};

class Site {
public:
	Customer *getCustomer() const {
		return _customer ? _customer : Customer::newNull();
	}

private:
	Customer *_customer;
};

Customer *Customer::newNull()
{
	return new NullCustomer();
}

int main()
{
	Site site;
	Customer *customer = site.getCustomer();

	BillingPlan plan = customer->getPlan();

	String customerName = customer->getName();

	int weeksDelinquent = customer->getHistory()->getWeeksDelinquentInLastYear();
}

