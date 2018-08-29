double price()
{
	// price is base price - quantity discount + shipping
	return _quantity * _itemPrice - 
		max(0, _quantity - 500) * _itemPrice * 0.05 +
		min(_quantity * _itemPrice * 0.1, 100.0);
}