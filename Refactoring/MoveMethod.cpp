class Account;

class AccountType {
public:
	bool isPremium() const;
	double overdraftCharge(const Account &account);
};

class Account {
	AccountType _type;
	int _daysOverdrawn;
public:
	double bankCharge() {
		double result = 4.5;
		if (_daysOverdrawn > 0)
			result += _type.overdraftCharge(*this);
		return result;
	}

	int getDaysOverdrawn() const {
		return _daysOverdrawn;
	}
};

double AccountType::overdraftCharge(const Account &account) {
	if (isPremium()) {
		double result = 10;
		if (account.getDaysOverdrawn() > 7)
			result += (account.getDaysOverdrawn() - 7) * 0.85;
		return result;
	} else return account.getDaysOverdrawn() * 1.75;
}
