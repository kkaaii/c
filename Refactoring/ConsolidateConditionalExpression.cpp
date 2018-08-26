bool isNotEligibleForDisability() {
	return (_seniority < 2) || (_monthsDisabled > 12) || (_isPartTime);
}

double disabilityAmount() {
	if (isNotEligibleForDisability()) return 0;
	// TODO: compiute the disability amount
}

return (onVacation() && lengthOfService() > 10) ? 1 : 0.5;

