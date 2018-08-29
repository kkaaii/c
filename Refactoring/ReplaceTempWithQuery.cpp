int basePrice()
{
	return _quantity * _itemPrice;
}

double discountFactor()
{
	if (basePrice() > 1000) return 0.95;
	else return 0.98;
}
double getPrice()
{
	return basePrice() * discountFactor();
}
