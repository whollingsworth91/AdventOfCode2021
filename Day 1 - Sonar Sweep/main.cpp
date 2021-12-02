#include <iostream>
#include <fstream>
#include <sstream>

#define PRINT_DEBUG_STATEMENTS 0

// part 1: How many measurements are larger than the previous measurement?
int getNumOfMeasurementsLargerThanPreviousMeasurements(std::string fpath) 
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for sonar measurements
	std::string line;
	int largerSonarMeasurementCount = 0;
	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		int sonarMeasurement;
		if (!(iss >> sonarMeasurement)) {
			break; // error
		}
		static int prevSonarMeasurement = sonarMeasurement;

		// process pair (a,b)
		if (sonarMeasurement > prevSonarMeasurement) {
			largerSonarMeasurementCount++;
#if PRINT_DEBUG_STATEMENTS
			std::cout << sonarMeasurement << " (increase)" << std::endl;
#endif
		}
		else {
#if PRINT_DEBUG_STATEMENTS
			std::cout << sonarMeasurement << " (decrease or same)" << std::endl;
#endif
		}
		prevSonarMeasurement = sonarMeasurement;
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return largerSonarMeasurementCount;
}

// part 2: How many sums are larger than the previous sum? (sums are groups of 3)
bool isCurrentGroupSumLargerThanPreviousGroupSum(int currSumCount, int prevSumCount)
{
	return (currSumCount > prevSumCount);
}

int getNumOfSumsLargerThanPreviousSum(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for sonar measurements
	int currGroupSumCount = 0;
	int prevGroupSumCount = 0;
	int totalLargerGroupSumCount = -1; // start at -1 for skipping sum of first group
	int groupIterator = 0;
	const int MAX_GROUP_ITER = 3;
	int groupSlidingWindow[3] { 0 };
	bool startCalculatingGroupSums = false;
	int sonarMeasurement;

	std::string line;
	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		if (!(iss >> sonarMeasurement)) {
			break; // error
		}

		// calculate sum of sliding window comparisons
		groupSlidingWindow[groupIterator] = sonarMeasurement;
		groupIterator++;

		if (groupIterator == MAX_GROUP_ITER) {
			startCalculatingGroupSums = true;
			groupIterator = 0;
		}

		if (startCalculatingGroupSums) {
			currGroupSumCount = groupSlidingWindow[0] + groupSlidingWindow[1] + groupSlidingWindow[2];
#if PRINT_DEBUG_STATEMENTS
			std::cout << "Sliding Window: " << groupSlidingWindow[0] << " + " << groupSlidingWindow[1] << " + " << groupSlidingWindow[2] << " = " << currGroupSumCount << (isCurrentSumCountLarger(currGroupSumCount, prevGroupSumCount) ? " (larger than) " : " (smaller than or same as) ") << prevGroupSumCount << std::endl;
#endif
		}

		if (isCurrentGroupSumLargerThanPreviousGroupSum(currGroupSumCount, prevGroupSumCount)) {
			totalLargerGroupSumCount++;
		}

		prevGroupSumCount = currGroupSumCount;
	}

	// close file after reading
	input.close();
	std::cout << std::endl;

	return totalLargerGroupSumCount;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\Day 1 - Sonar Sweep\\sonar measurements.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/1
	int answer1 = getNumOfMeasurementsLargerThanPreviousMeasurements(fpath);
	int answer2 = getNumOfSumsLargerThanPreviousSum(fpath);

	std::cout << "How many measurements are larger than the previous measurement?: " << answer1 << std::endl << std::endl;
	std::cout << "How many sums are larger than the previous sum?: " << answer2 << std::endl << std::endl;
}