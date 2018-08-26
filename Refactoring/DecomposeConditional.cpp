bool notSummer(Date date) {
	return date.before(SUMMER_START) || date.after(SUMMER_END);
}

double summerCharge(int quantity) {
	return quantity * _summerRate;
}

double winterCharge(int quantity) {
	return quantity * _winterRate + _winterServiceCharge;
}

if (notSummer(date))
	charge = summerCharge(quantity);
else
	charge = winterCharge(quantity);

