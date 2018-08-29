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

class Customer;
class Statement {
public:
	String value(const Customer &customer) const;
	virtual String headerString(const Customer &customer) const = 0;
	virtual String footerString(const Customer &customer) const = 0;
	virtual String eachRentalString(const Rental &rental) const = 0;
};

class TextStatement: public Statement {
public:
	virtual String headerString(const Customer &customer) const;
	virtual String footerString(const Customer &customer) const;
	virtual String eachRentalString(const Rental &rental) const;
};

class HtmlStatement: public Statement {
public:
	virtual String headerString(const Customer &customer) const;
	virtual String footerString(const Customer &customer) const;
	virtual String eachRentalString(const Rental &rental) const;
};

typedef std::vector<Rental> Rentals;
typedef Rentals::const_iterator Iterator;

class Customer {
	Rentals _rentals;
public:
	String getName() const;
	Rentals& getRentals() const;
	String getTotalCharge() const;
	String getTotalFrequentRenterPoints() const;

	String statement() const {
		return TextStatement().value(*this);
	}
	String htmlStatement() const {
		return HtmlStatement().value(*this);
	}
};

String Statement::value(const Customer &customer) const
{
	String result = headerString(customer);
	Rentals &rentals = customer.getRentals();
	for (Iterator it = rentals.begin(); it != rentals.end(); ++it) {
		result += eachRentalString(*it);
	}
	return result + footerString(customer);
}

String TextStatement::headerString(const Customer &customer) const
{
	return "Rental Record for " + customer.getName() + "\n";
}

String TextStatement::eachRentalString(const Rental &rental) const
{
	return "\t" + rental.getMovie().getTitle() + "\t" + rental.getCharge() + "\n";
}

String TextStatement::footerString(const Customer &customer) const
{
	return "Amount owed is " + String(customer.getTotalCharge()) + "\n" +
		"You earned " + String(customer.getTotalFrequentRenterPoints()) + " frequent renter points";
}

String HtmlStatement::headerString(const Customer &customer) const
{
	return "<H1>Rentals for <EM>" + customer.getName() + "</EM></H1><P>\n";
}

String HtmlStatement::eachRentalString(const Rental &rental) const
{
	return rental.getMovie().getTitle() + ": " + rental.getCharge() + "<BR>\n";
}

String HtmlStatement::footerString(const Customer &customer) const
{
	return "<P> You owe <EM>" + String(customer.getTotalCharge()) + "</EM>\n" +
		"<P>On this rental you earned <EM>" + String(customer.getTotalFrequentRenterPoints()) + "</EM> frequent renter points\n";
}
