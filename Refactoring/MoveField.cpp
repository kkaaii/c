class AccountType {};

class Account {
	AccountType &_type;
	double _interestRate;
	
	double interestForAmount_days(double amount, int days) {
		return _interestRate * amount * days / 365;
	}
};