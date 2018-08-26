double disabilityAmount() {
	if (_seniority < 2) return 0;
	if (_monthsDisabled > 12) return 0;
	if (_isPartTime) return 0;
	// TODO: compiute the disability amount
}

if (onVacation())
	if (lengthOfService() > 10)
		return 1;
return 0.5;

