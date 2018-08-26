class Person {
public:
	virtual ~Person() {}
	virtual bool isMale() const = 0;
	virtual char getCode() const = 0;
};

class Male: public Person {
	virtual bool isMale() const {
		return true;
	}

	virtual char getCode() const {
		return 'M';
	}
};

class Female: public Person {
	virtual bool isMale() const {
		return false;
	}

	virtual char getCode() const {
		return 'F';
	}
};

