static double discountedPrice(int basePrice, int discountLevel)
{
	if (discountLevel == 2) return basePrice * 0.9;
	else return basePrice * 0.95;
}
double getPrice()
{
	int basePrice = _quantity * _itemPrice;
	int discountLevel;

	if (_quantity > 100) discountLevel = 2;
	else discountLevel = 1;

	double finalPrice = discountedPrice(basePrice, discountLevel);
	return finalPrice;
}

