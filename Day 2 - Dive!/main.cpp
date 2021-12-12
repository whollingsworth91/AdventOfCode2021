#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

#define PRINT_DEBUG_STATEMENTS 0

// part 1: What do you get if you multiply your final horizontal position by your final depth?
int getHorizontalPosByDepth(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for aim calculation
	std::string line;
	int horizontalPos = 0;
	int depthPos = 0;
	std::string command;
	int value = 0;

	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		if (!(iss >> command >> value)) {
			break; // error
		}

		// calculate commands
		if (command.compare("forward") == 0) {
			horizontalPos += value;
		}
		else if (command.compare("down") == 0) {
			depthPos += value;
		}
		else if (command.compare("up") == 0) {
			depthPos -= value;
		}

	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return horizontalPos * depthPos;
}

// part 2: What do you get if you multiply your final horizontal position by your final depth using aim?
int getMoreComplexHorizontalPosByDepthUsingAim(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for aim calculation
	std::string line;
	int horizontalPos = 0;
	int depthPos = 0;
	std::string command;
	int value = 0;
	int aim = 0;

	while (std::getline(input, line))
	{
		std::istringstream iss(line);
		if (!(iss >> command >> value)) {
			break; // error
		}

		// calculate commands
		if (command.compare("forward") == 0) {
			horizontalPos += value;
			depthPos += (aim * value);
		}
		else if (command.compare("down") == 0) {
			aim += value;
		}
		else if (command.compare("up") == 0) {
			aim -= value;
		}
		else if (command.compare("aim") == 0) {
			depthPos -= value;
		}
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	return horizontalPos * depthPos;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\Day 2 - Dive!\\dive.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/2
	int answer1 = getHorizontalPosByDepth(fpath);
	int answer2 = getMoreComplexHorizontalPosByDepthUsingAim(fpath);

	std::cout << "What do you get if you multiply your final horizontal position by your final depth?: " << answer1 << std::endl << std::endl;
	std::cout << "What do you get if you multiply your final horizontal position by your final depth using aim?: " << answer2 << std::endl << std::endl;

	// sleep to keep cmd window open
	std::string in;
	while (in.compare("exit") != 0) {
		std::cin >> in;
		std::this_thread::sleep_for(2000ms);
	}
}