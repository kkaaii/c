#include <string>
#include <vector>

typedef std::string String;
typedef std::vector<String> PersonList;

String foundPerson(const PersonList &people)
{
	PersonList candidates;
	candidates.push_back("Don");
	candidates.push_back("John");
	candidates.push_back("Kent");

	for (int i = 0; i < people.size(); ++i) {
		if (candidates.contains(people[i]))
			return people[i];
	}
	
	return "";
}