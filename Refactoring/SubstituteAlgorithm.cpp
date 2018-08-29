#include <string>
#include <vector>

typedef std::string String;
typedef std::vector<String> PersonList;

String foundPerson(const PersonList &people)
{
	for (int i = 0; i < people.size(); ++i) {
		if (people[i] == "Don") {
			return "Don";
		}
		if (people[i] == "John") {
			return "John";
		}
		if (people[i] == "Kent") {
			return "Kent";
		}
	}
	
	return "";
}