class AccountType {
	double _interestRate;
public:
	double getInterestRate() const {
		return _interestRate;
	}
};

class Account {
	AccountType &_type;
	
	double interestForAmount_days(double amount, int days) {
		return _type.getInterestRate() * amount * days / 365;
	}
};