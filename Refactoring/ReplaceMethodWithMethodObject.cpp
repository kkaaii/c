class Account;
class Gamma {
	const Account &_account;
	int inputVal;
	int quantity;
	int yearToDate;
	int importantValue1;
	int importantValue2;
	int importantValue3;
public:
	Gamma(const Account &account, int inputValArg, int quantityArg, int yearToDateArg):
		_account(account), inputVal(inputValArg), quantity(quantityArg), yearToDate(yearToDateArg) {
		}
	
	int compute() {
		int importantValue1 = (inputVal * quantity) + _account.delta();
		int importantValue2 = (inputVal * yearToDate) + 100;
		if ((yearToDate - importantValue1) > 100)
			importantValue2 -= 20;
		int importantValue3 = importantValue2 * 7;
		// and so on
		return importantValue3 - 2 * importantValue1;
	}
};

class Account {
public:
	int gamma(int inputVal, int quantity, int yearToDate)
	{
		return Gamma(*this, inputVal, quantity, yearToDate).compute();
	}
};