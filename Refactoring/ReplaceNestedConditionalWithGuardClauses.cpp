double getPayAmount()
{
	if (_isDead) return deadAmount();
	if (_isSeparated) return separatedAmount();
	if (_isRetired) return retiredAmount();
	return normalAmount();
}

double getAdjustedCapital()
{
	if (_capital <= 0.0) return 0.0;
	if (_intRate <= 0.0 || _duration <= 0.0) return 0.0;
	return (_income / _duration) * ADJ_FACTOR;
}

