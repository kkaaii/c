class Person {
public:
	Person(bool isMale, char code):
		_isMale(isMale), _code(code) {
	}

	bool isMale() const {
		return _isMale;
	}

	virtual char getCode() const {
		return _code;
	}

	static Person *createMale();
	static Person *createFemale();

private:
	bool _isMale;
	char _code;
};

Person *Person::createMale() {
	return new Person(true, 'M');
}

Person *Person::createFemale() {
	return new Person(false, 'F');
}

