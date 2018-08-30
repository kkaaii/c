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
	CourseSet _courses;
public:
	int numberOfCourses() const {
		return _courses.size();
	}

	int numberOfAdvancedCourses() const {
		int count = 0;
		CourseSet::const_iterator it = _courses.begin();
		while (it != _courses.end()) {
			if ((*it)->isAdvanced()) ++count;
			++it;
		}
		return count;
	}

	void addCourse(Course *course) {
		_courses.insert(course);
	}

	void removeCourse(Course *course) {
		_courses.erase(_courses.find(course));
	}
};

void foo()
{
	Person kent;
	kent.addCourse(new Course("Smalltalk Programming", false));
	kent.addCourse(new Course("Appreciating Single Malts", true));
	LONGS_EQUAL(2, kent.numberOfCourses());

	Course *refact = new Course("Refactoring", true);
	kent.addCourse(refact);
	kent.addCourse(new Course("Brutal Sarcasm", false));
	LONGS_EQUAL(4, kent.numberOfCourses());

	kent.removeCourse(refact);
	LONGS_EQUAL(3, kent.numberOfCourses());

	int count = kent.numberOfAdvancedCourses();
}