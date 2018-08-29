#include <vector>

typedef std::vector<int> Vector;

class Stack: public Vector {
public:
	void push(int element) {
		push_back(element);
	}
	
	int pop() {
		int element = back();
		pop_back();
		return element;
	}
};