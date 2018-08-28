#include <string>
#include <vector>

typedef std::string String;

extern void sendAlert(void);
extern void someLaterCode(String &);

String foundMiscreant(std::vector<String> &people)
{
	for (int i = 0; i < people.size(); ++i) {
		if (people[i] == "Don") {
			sendAlert();
			return "Don";
		}

		if (people[i] == "John") {
			sendAlert();
			return "John";
		}
	}

	return "";
}

void checkSecurity(std::vector<String> &people)
{
	String found = foundMiscreant(people);
	someLaterCode(found);
}

