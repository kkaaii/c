void checkSecurity(String people[])
{
	bool found = false;

	for (int i = 0; i < people.length(); ++i) {
		if (!found) {
			if (people[i].equals("Don")) {
				sendAlert();
				found = true;
			}

			if (people[i].equals("John")) {
				sendAlert();
				found = true;
			}
		}
	}
}

void checkSecurity(String people[])
{
	String found = "";

	for (int i = 0; i < people.length(); ++i) {
		if (found.equals("")) {
			if (people[i].equals("Don")) {
				sendAlert();
				found = "Don";
			}

			if (people[i].equals("John")) {
				sendAlert();
				found = "John";
			}
		}
	}

	someLaterCode(found);
}

