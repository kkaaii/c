double price()
{
	const double basePrice = _quantity * _itemPrice;
	const double quantityDiscount = max(0, _quantity - 500) * _itemPrice * 0.05;
	const double shipping = min(basePrice * 0.1, 100.0);
	return basePrice - quantityDiscount + shipping;
}