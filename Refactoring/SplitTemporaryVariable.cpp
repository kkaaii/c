double getDistanceTravelled(int time)
{
	double result;
	const double primaryAcc = _primaryForce / _mass;
	int primaryTime = min(time, _delay);
	result = 0.5 * primaryAcc * primaryTime * primaryTime;
	int secondaryTime = time - _delay;
	if (secondaryTime > 0) {
		double primaryVel = primaryAcc * _delay;
		const double secondaryAcc = (_primaryForce + _secondaryForce)  / _mass;
		result += primaryVel * secondaryTime + 0.5 * secondaryAcc * secondaryTime * secondaryTime;
	}
	return result;
}