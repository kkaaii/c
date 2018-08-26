class Person {
	int _bloodGroup;
public:
	enum {O = 0, A = 1, B = 2, AB = 3};

	Person(int bloodGroup): _bloodGroup(bloodGroup) {}

	void setBloodGroup(int bloodGroup) {
		_bloodGroup = bloodGroup;
	}

	int getBloodGroup(void) {
		return _bloodGroup;
	}
};

