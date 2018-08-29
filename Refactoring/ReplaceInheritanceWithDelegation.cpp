#include <vector>

typedef std::vector<int> Vector;

class Stack {
	Vector &_vector;
public:
	void push(int element) {
		_vector.push_back(element);
	}

	int pop() {
		int element = _vector.back();
		_vector.pop_back();
		return element;
	}
	
	int size() const {
		return _vector.size();
	}
	
	bool empty() const {
		return _vector.empty();
	}
};