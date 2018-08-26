class BloodGroup {
	int _code;
	static BloodGroup O;
	static BloodGroup A;
	static BloodGroup B;
	static BloodGroup AB;
public:
	BloodGroup(int code): _code(code) {}

private:
	int getCode(void) {
		return _code;
	}
};

BloodGroup BloodGroup::O = BloodGroup(0);
BloodGroup BloodGroup::A = BloodGroup(1);
BloodGroup BloodGroup::B = BloodGroup(2);
BloodGroup BloodGroup::AB = BloodGroup(3);

class Person {
	BloodGroup &_bloodGroup;
public:
	Person(BloodGroup &bloodGroup): _bloodGroup(bloodGroup) {}

	void setBloodGroup(BloodGroup &bloodGroup) {
		_bloodGroup = bloodGroup;
	}

	BloodGroup &getBloodGroup(void) {
		return _bloodGroup;
	}
};

