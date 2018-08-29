#include <iostream>
using namespace std;

void printBanner()
{
	// print banner
	cout << "************************" << endl;
	cout << "***** Customer Owes *****" << endl;
	cout << "************************" << endl;
}

void printDetails(double outstanding)
{
	// print details
	cout << "name: " << _name << endl;
	cout << "amount: " << outstanding << endl;
}

double getOutstanding(double initialValue)
{
	// calculate outstanding
	double result = initialValue;
	for (it = _orders.begin(); it != _orders.end(); ++it) {
		result += it->getAmount();
	}
	return result;
}

void printOwing(double previousAmount)
{
	printBanner();
	double outstanding = getOutstanding(previousAmount * 1.2);
	printDetails(outstanding);
}
