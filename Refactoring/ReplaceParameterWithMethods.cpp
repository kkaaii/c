static double getBasePrice()
{
	return _quantity * _itemPrice;
}

static int getDiscountLevel()
{
	return (_quantity > 100) ? 2 : 1;
}

double getPrice()
{
	if (getDiscountLevel() == 2) return getBasePrice() * 0.9;
	else return getBasePrice() * 0.95;
}

