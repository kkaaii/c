#include <string>

typedef std::string String;

class Interval: public Observable {
	String _start;
	String _end;
	String _length;
public:
	String getStart() const {
		return _start;
	}

	String getEnd() const {
		return _end;
	}

	String getLength() const {
		return _length;
	}

	void setStart(const String &text) {
		_start = text;
	}

	void setEnd(const String &text) {
		_end = text;
	}

	void setLength(const String &text) {
		_length = text;
	}
};

class TextField {
public:
	String getText() const;
	void setText(String text);
};

class IntervalWindow {
	Interval *_subject;
	TextField _startField;
	TextField _endField;
	TextField _lengthField;
public:
	IntervalWindow() {
		_subject = new Interval();
		_subject->addObserver(this);
	}
protected:
	String getStart() const {
		return _subject->getStart();
	}

	String getEnd() const {
		return _subject->getEnd();
	}

	String getLength() const {
		return _subject->getLength();
	}

	void setStart(String text) {
		_subject->setStart(text);
	}

	void setEnd(String text) {
		_subject->setEnd(text);
	}

	void setLength(String text) {
		_subject->setLength(text);
	}

	void StartField_FocusLost() {
		setStart(_startField.getText());
		if (isNotInteger(getStart()))
			setStart("0");
		calculateLength();
	}
	
	void EndField_FocusLost() {
		setEnd(_endField.getText());
		if (isNotInteger(getEnd()))
			setEnd("0");
		calculateLength();
	}
	
	void LengthField_FocusLost() {
		setLength(_lengthField.getText());
		if (isNotInteger(getLength()))
			setLength("0");
		calculateEnd();
	}
	
	void calculateLength() {
		int start = StringToInt(getStart());
		int end = StringToInt(getEnd());
		int length = end - start;
		setLength(IntToString(length));
	}
	
	void calculateEnd() {
		int start = StringToInt(getStart());
		int length = StringToInt(getLength());
		int end = start + length;
		setEnd(IntToString(end));
	}
};