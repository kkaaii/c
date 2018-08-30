#include <string>
#include <set>

typedef std::string String;

class Course {
public:
	Course(String name, bool isAdvanced);
	bool isAdvanced() const;
};

typedef std::set<Course *> CourseSet;

class Person {
	CourseSet *_courses;
public:
	CourseSet *getCourses() const {
		return _courses;
	}

	void setCourses(CourseSet *courses) {
		_courses = courses;
	}
};

void foo()
{
	CourseSet s;
	s.insert(new Course("Smalltalk Programming", false));
	s.insert(new Course("Appreciating Single Malts", true));

	Person kent;
	kent.setCourses(&s);
	LONGS_EQUAL(2, kent.getCourses()->size());

	Course *refact = new Course("Refactoring", true);
	kent.getCourses()->insert(refact);
	kent.getCourses()->insert(new Course("Brutal Sarcasm", false));
	LONGS_EQUAL(4, kent.getCourses()->size());

	kent.getCourses()->erase(kent.getCourses()->find(refact));
	LONGS_EQUAL(3, kent.getCourses()->size());

	int count = 0;
	CourseSet::iterator it = kent.getCourses()->begin();
	while (it != kent.getCourses()->end()) {
		if ((*it)->isAdvanced()) ++count;
		++it;
	}
}