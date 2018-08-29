#include <string>
#include <vector>

typedef std::string String;

class Movie {
public:
	String getTitle() const;
};

class Rental {
public:
	Movie& getMovie() const;
	String getCharge() const;
};

class Customer {
	typedef std::vector<Rental> Rentals;
	typedef Rentals::const_iterator Iterator;
	Rentals _rentals;
public:
	String getName() const;
	String getTotalCharge() const;
	String getTotalFrequentRenterPoints() const;

	String statement() const;
	String htmlStatement() const;
};

String Customer::statement() const
{
	String result = "Rental Record for " + getName() + "\n";
	for (Iterator it = _rentals.begin(); it != _rentals.end(); ++it) {
		result += "\t" + it->getMovie().getTitle() + "\t" + it->getCharge() + "\n";
	}
	result += "Amount owed is " + String(getTotalCharge()) + "\n";
	result += "You earned " + String(getTotalFrequentRenterPoints()) + " frequent renter points";
	return result;
}

String Customer::htmlStatement() const
{
	String result = "<H1>Rentals for <EM>" + getName() + "</EM></H1><P>\n";
	for (Iterator it = _rentals.begin(); it != _rentals.end(); ++it) {
		result += it->getMovie().getTitle() + ": " + it->getCharge() + "<BR>\n";
	}
	result += "<P> You owe <EM>" + String(getTotalCharge()) + "</EM>\n";
	result += "<P>On this rental you earned <EM>" + String(getTotalFrequentRenterPoints()) + "</EM> frequent renter points\n";
	return result;
}
