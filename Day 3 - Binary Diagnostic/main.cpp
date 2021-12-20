#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>

using namespace std::chrono_literals;

#define PRINT_DEBUG_STATEMENTS 0

// part 1: What is the power consumption of the submarine?
int getPowerConsumption(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for power consumption
	std::string line;
	int epsilonRate = 0;
	int gammaRate = 0;
	std::string binaryStr;
	std::string epsilonRateStr;
	std::string gammaRateStr;
	std::vector<int> numOfOnes = std::vector<int>();
	std::vector<int> numOfZeroes = std::vector<int>();
	bool setSizeOfIntVectors = false;

	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		if (!(iss >> binaryStr)) {
			break; // error
		}

		if (!setSizeOfIntVectors) {
			for (int i = 0; i < binaryStr.length(); i++) {
				numOfOnes.push_back(0);
				numOfZeroes.push_back(0);
			}
			setSizeOfIntVectors = true;
		}

		// calculate '1' and '0' prevalence
		for (int i = 0; i < binaryStr.length(); i++) {
			(binaryStr.at(i) == '1') ? numOfOnes.at(i)++ : numOfZeroes.at(i)++;
		}
	}

	for (int i = 0; i < numOfOnes.size(); i++) {
		// gamma uses MSB
		if (numOfOnes.at(i) > numOfZeroes.at(i)) {
			gammaRateStr.push_back('1');
		}
		else {
			gammaRateStr.push_back('0');
		}

		// epsilon uses LSB
		if (numOfOnes.at(i) < numOfZeroes.at(i)) {
			epsilonRateStr.push_back('1');
		}
		else {
			epsilonRateStr.push_back('0');
		}
	}

	gammaRate = std::stoi(gammaRateStr, nullptr, 2);
	epsilonRate = std::stoi(epsilonRateStr, nullptr, 2);

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return epsilonRate * gammaRate;
}

// part 2: What is the life support rating of the submarine?
int getLifeSupportRating(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for life support rating
	std::string line;
	std::string binaryStr;
	size_t binLength = 0;

	std::string oxygenGeneratorRatingStr;
	std::string co2ScrubberRatingStr;
	std::list<std::string> oxyGenRatingStrs = std::list<std::string>();
	std::list<std::string> co2ScrubberRatingStrs = std::list<std::string>();
	int oxygenGeneratorRating = 0;
	int co2ScrubberRating = 0;

	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		if (!(iss >> binaryStr)) {
			break; // error
		}

		oxyGenRatingStrs.push_back(binaryStr);
		co2ScrubberRatingStrs.push_back(binaryStr);
	}

	binLength = binaryStr.length();

	size_t i = 0;
	while (i < binLength) {
		// calculate oxygen rating
		if (oxyGenRatingStrs.size() > 1) {
			// calculate '1' and '0' prevalence
			int64_t numOnes = 0;
			int64_t numZeroes = std::count_if(oxyGenRatingStrs.begin(), oxyGenRatingStrs.end(), [&i, &numOnes](const std::string& str) {
				if (str.at(i) == '0') {
					return true;
				}
				numOnes++;
				return false;
			});

			// To find oxygen generator rating, determine the most common value(0 or 1) in the current bit position, and keep only numbers with that bit in that position.
			// If 0 and 1 are equally common, keep values with a 1 in the position being considered.
			const char mostCommonBit = (numOnes >= numZeroes) ? '1' : '0';
			auto it = std::remove_if(oxyGenRatingStrs.begin(), oxyGenRatingStrs.end(), [&i, &mostCommonBit](const std::string& str) {
				if (str.at(i) == mostCommonBit) {
					return false;
				}
				return true;
			});
			oxyGenRatingStrs.erase(it, oxyGenRatingStrs.end());
		}

		// calculate CO2 scrubber rating
		if (co2ScrubberRatingStrs.size() > 1) {
			// calculate '1' and '0' prevalence
			int64_t numOnes = 0;
			int64_t numZeroes = std::count_if(co2ScrubberRatingStrs.begin(), co2ScrubberRatingStrs.end(), [&i, &numOnes](const std::string& str) {
				if (str.compare("") != 0 && str.at(i) == '0') {
					return true;
				}
				numOnes++;
				return false;
			});

			// To find CO2 scrubber rating, determine the least common value(0 or 1) in the current bit position, and keep only numbers with that bit in that position.
			// If 0 and 1 are equally common, keep values with a 0 in the position being considered.
			const char leastCommonBit = (numOnes < numZeroes) ? '1' : '0';
			auto it = std::remove_if(co2ScrubberRatingStrs.begin(), co2ScrubberRatingStrs.end(), [&i, &leastCommonBit](const std::string& str) {
				if (str.at(i) == leastCommonBit) {
					return false;
				}
				return true;
			});

			co2ScrubberRatingStrs.erase(it, co2ScrubberRatingStrs.end());
		}

		i++;
	}

	oxygenGeneratorRatingStr = oxyGenRatingStrs.front();
	oxygenGeneratorRating = std::stoi(oxygenGeneratorRatingStr, nullptr, 2);

	co2ScrubberRatingStr = co2ScrubberRatingStrs.front();
	co2ScrubberRating = std::stoi(co2ScrubberRatingStr, nullptr, 2);

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return oxygenGeneratorRating * co2ScrubberRating;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\Day 3 - Binary Diagnostic\\binary_diagnostic.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/3
	int answer1 = getPowerConsumption(fpath);
	int answer2 = getLifeSupportRating(fpath);

	std::cout << "What is the power consumption of the submarine?: " << answer1 << std::endl << std::endl;
	std::cout << "What is the life support rating of the submarine?: " << answer2 << std::endl << std::endl;

	// sleep to keep cmd window open
	std::string in;
	while (in.compare("exit") != 0) {
		std::cin >> in;
		std::this_thread::sleep_for(2000ms);
	}
}