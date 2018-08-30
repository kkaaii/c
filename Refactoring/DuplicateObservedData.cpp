#include <string>

typedef std::string String;

class TextField {
public:
	String getText() const;
	void setText(String text);
};

class IntervalWindow {
	TextField _startField;
	TextField _endField;
	TextField _lengthField;
protected:
	void StartField_FocusLost() {
		if (isNotInteger(_startField.getText()))
			_startField.setText("0");
		calculateLength();
	}
	
	void EndField_FocusLost() {
		if (isNotInteger(_endField.getText()))
			_endField.setText("0");
		calculateLength();
	}
	
	void LengthField_FocusLost() {
		if (isNotInteger(_lengthField.getText()))
			_lengthField.setText("0");
		calculateEnd();
	}
	
	void calculateLength() {
		int start = StringToInt(_startField.getText());
		int end = StringToInt(_endField.getText());
		int length = end - start;
		_lengthField.setText(IntToString(length));
	}
	
	void calculateEnd() {
		int start = StringToInt(_startField.getText());
		int length = StringToInt(_lengthField.getText());
		int end = start + length;
		_endField.setText(IntToString(end));
	}
};