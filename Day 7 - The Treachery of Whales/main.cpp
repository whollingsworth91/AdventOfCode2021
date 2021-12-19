#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#define PRINT_DEBUG_STATEMENTS 0

// part 1: How much fuel must they spend to align to that position?
int getHowMuchFuelToAlignPos(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for aim calculation
	std::string line;
	int totalFuelCost = 0;
	int lowestFuelCost = INT_MAX;

	std::vector<int> horizontalPositions = std::vector<int>();

	while (std::getline(input, line))
	{
		int pos = 0;
		std::string delimiter = ",";
		while ((pos = line.find(delimiter)) != std::string::npos) {
			horizontalPositions.push_back(std::stoi(line.substr(0, pos), nullptr, 10));
			line.erase(0, pos + delimiter.length());
		}

		horizontalPositions.push_back(std::stoi(line, nullptr, 10));
	}

	// process input
	for (int i = 0; i < horizontalPositions.size(); i++) {
		for (int j = 0; j < horizontalPositions.size(); j++) {
			totalFuelCost += abs(horizontalPositions[i] - horizontalPositions[j]);
		}

		if (totalFuelCost < lowestFuelCost) {
			lowestFuelCost = totalFuelCost;
		}
		totalFuelCost = 0;
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return lowestFuelCost;
}

int sumOfParts(int sumOfLesserNums) {
	int ret = 0;
	if (sumOfLesserNums <= 0) {
		return ret;
	}

	for (int i = 0; i <= sumOfLesserNums; i++) {
		ret += i;
	}

	return ret;
}

// part 2: How much fuel must they spend to align to that position (expensive version)?
int getHowMuchExpensiveFuelToAlignPos(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for aim calculation
	std::string line;
	int totalFuelCost = 0;
	int lowestFuelCost = INT_MAX;
	int alignmentOfCheapestPosition = 0;

	std::vector<int> horizontalPositions = std::vector<int>();

	while (std::getline(input, line))
	{
		int pos = 0;
		std::string delimiter = ",";
		while ((pos = line.find(delimiter)) != std::string::npos) {
			horizontalPositions.push_back(std::stoi(line.substr(0, pos), nullptr, 10));
			line.erase(0, pos + delimiter.length());
		}

		horizontalPositions.push_back(std::stoi(line, nullptr, 10));
	}

	// process input
	for (int i = 0; i < horizontalPositions.size(); i++) {
		for (int j = 0; j < horizontalPositions.size(); j++) {
			totalFuelCost += sumOfParts(abs(i - horizontalPositions[j]));
#if PRINT_DEBUG_STATEMENTS
			printf("Move from %d to %d: %d fuel cost\n", i, horizontalPositions[j], sumOfParts(abs(i - horizontalPositions[j])));
#endif
		}

#if PRINT_DEBUG_STATEMENTS
		printf(" %d total fuel cost\n\n", totalFuelCost);
#endif
		if (totalFuelCost < lowestFuelCost) {
			alignmentOfCheapestPosition = i;
			lowestFuelCost = totalFuelCost;
		}
		totalFuelCost = 0;
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return lowestFuelCost;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\..\\Day 7 - The Treachery of Whales\\horizontal_positions.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/7
	int answer1 = getHowMuchFuelToAlignPos(fpath);
	int answer2 = getHowMuchExpensiveFuelToAlignPos(fpath);

	std::cout << "How much fuel must they spend to align to that position?: " << answer1 << std::endl << std::endl;
	std::cout << "How much expensive fuel must they spend to align to that position?: " << answer2 << std::endl << std::endl;
}