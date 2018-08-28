#include <string>
#include <vector>

typedef std::string String;

extern void sendAlert(void);
extern void someLaterCode(String &);

String foundMiscreant(std::vector<String> &people)
{
	for (int i = 0; i < people.size(); ++i) {
		if (people[i] == "Don") {
			return "Don";
		}

		if (people[i] == "John") {
			return "John";
		}
	}

	return "";
}

void checkSecurity(std::vector<String> &people)
{
	String found = foundMiscreant(people);
	if (found != "")
		sendAlert();
	someLaterCode(found);
}

