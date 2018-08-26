double getPayAmount()
{
	double result;

	if (_isDead) result = deadAmount();
	else {
		if (_isSeparated) result = separatedAmount();
		else {
			if (_isRetired) result = retiredAmount();
			else result = normalAmount();
		}
	}

	return result;
}

double getAdjustedCapital()
{
	double result = 0.0;

	if (_capital > 0.0) {
		if (_intRate > 0.0 && _duration > 0.0) {
			result = (_income / _duration) * ADJ_FACTOR;
		}
	}

	return result;
}

