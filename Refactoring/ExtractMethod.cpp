#include <iostream>
using namespace std;

void printOwing(double previousAmount)
{
	// print banner
	cout << "************************" << endl;
	cout << "***** Customer Owes *****" << endl;
	cout << "************************" << endl;
	
	// calculate outstanding
	double outstanding = previousAmount * 1.2;
	for (it = _orders.begin(); it != _orders.end(); ++it) {
		outstanding += it->getAmount();
	}
	
	// print details
	cout << "name: " << _name << endl;
	cout << "amount: " << outstanding << endl;
}
