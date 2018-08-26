#include <string>

typedef typename std::string String;

class BillingPlan {
};

class PaymentHistory {
public:
	int getWeeksDelinquentInLastYear() const {
		return _weeksDelinquentInLastYear;
	}

private:
	int _weeksDelinquentInLastYear;
};

class Customer {
public:
	String getName() const;
	BillingPlan getPlan() const;
	PaymentHistory *getHistory() const;
};

class Site {
public:
	Customer *getCustomer() const {
		return _customer;
	}

private:
	Customer *_customer;
};

int main()
{
	Site site;
	Customer *customer = site.getCustomer();

	BillingPlan plan;
	if (0 == customer) plan = BillingPlan::basic();
	else plan = customer->getPlan();

	String customerName;
	if (0 == customer) customerName = "occupant";
	else customerName = customer->getName();

	int weeksDelinquent;
	if (0 == customer) weeksDelinquent = 0;
	else weeksDelinquent = customer->getHistory()->getWeeksDelinquentInLastYear();
}

