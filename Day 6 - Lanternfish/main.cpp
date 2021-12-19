#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define PRINT_DEBUG_STATEMENTS 0
typedef unsigned long long ull;

// part 1 & part 2: How many lanternfish would there be after N days?
ull getNumLanternfishAtNDays(std::string fpath, int afterHowManyDays)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file at \"" << fpath << "\"" << std::endl;
	}

	// parse file for aim calculation
	std::string line;
	std::string ageOfLanternfish;
	std::vector<ull> fishAges = std::vector<ull>(9);

	while (std::getline(input, line))
	{
		int pos = 0;
		std::string delimiter = ",";
		while ((pos = line.find(delimiter)) != std::string::npos) {
			fishAges[std::stoi(line.substr(0, pos), nullptr, 10)] += 1;
			line.erase(0, pos + delimiter.length());
		}

		fishAges[std::stoi(line, nullptr, 10)] += 1;
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	// process input
	std::vector<ull> numTherePreviousDay = std::vector<ull>(fishAges);
	printf("Day 0: %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld", fishAges[0], fishAges[1], fishAges[2], fishAges[3], fishAges[4], fishAges[5], fishAges[6], fishAges[7], fishAges[8]);

	for (int i = 0; i < afterHowManyDays; i++) {
		// Each day, a 0 becomes a 6 and adds a new 8 to the end of the list, 
		// while each other number decreases by 1 if it was present at the start of the day.
#if PRINT_DEBUG_STATEMENTS
		printf("\nDay %d: %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld", i, fishAges[0], fishAges[1], fishAges[2], fishAges[3], fishAges[4], fishAges[5], fishAges[6], fishAges[7], fishAges[8]);
#endif
		fishAges[6] += fishAges[0]; // zero becomes a 6
		fishAges[8] += fishAges[0]; // new 8 added to the end of the list
		fishAges[1] -= numTherePreviousDay[1];
		fishAges[0] = numTherePreviousDay[1];

		for (int j = 2; j < fishAges.size(); j++) {
			fishAges[j] -= numTherePreviousDay[j];
			fishAges[j-1] += numTherePreviousDay[j];
		}

		numTherePreviousDay.clear();
		numTherePreviousDay = std::vector<ull>(fishAges);
	}
	printf("\nDay %d (last day): %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld", afterHowManyDays, fishAges[0], fishAges[1], fishAges[2], fishAges[3], fishAges[4], fishAges[5], fishAges[6], fishAges[7], fishAges[8]);
	printf("\n");

	// count lanternfish
	ull fishAgeCount = 0;
	for (int i = 0; i < fishAges.size(); i++) {
		fishAgeCount += fishAges[i];
	}

	return fishAgeCount;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\..\\Day 6 - Lanternfish\\lanternfish_ages.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/6
	int howManyDaysQ1 = 80;
	int howManyDaysQ2 = 256;
	ull answer1 = getNumLanternfishAtNDays(fpath, howManyDaysQ1);
	printf("How many lanternfish would there be after %d days?: %llu\n", howManyDaysQ1, answer1);

	ull answer2 = getNumLanternfishAtNDays(fpath, howManyDaysQ2);
	printf("How many lanternfish would there be after %d days?: %llu\n", howManyDaysQ2, answer2);

	std::string cin;
	std::cin >> cin;
}