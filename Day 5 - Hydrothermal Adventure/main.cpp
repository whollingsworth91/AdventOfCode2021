#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#define PRINT_DEBUG_STATEMENTS 0

struct Vec2D {
	int horizontal = 0;
	int vertical = 0;

	Vec2D(){ }

	Vec2D(int x, int y) {
		horizontal = x;
		vertical = y;
	}
};

//Vec2D operator + (Vec2D const& c1, Vec2D const& c2)
//{
//	return Vec2D(c1.horizontal + c2.horizontal, c1.vertical + c2.vertical);
//}

bool isDiagonal(Vec2D start, Vec2D end) {
	if (start.horizontal == start.vertical && end.horizontal == end.vertical) {
		return true;
	}
	if (start.horizontal == end.vertical && start.vertical == end.horizontal) {
		return true;
	}
	if (abs(start.horizontal - end.horizontal) == abs(start.vertical - end.vertical)) {
		return true;
	}
	return false;
}

bool isALargerthanB(int a, int b) {
	return (a > b);
}

Vec2D diff(Vec2D start, Vec2D end, bool* startHorizontalLargerThanEndHorizontal, bool* startVerticalLargerThanEndVertical) {
	Vec2D ret;

	ret.horizontal = start.horizontal - end.horizontal;
	if (startHorizontalLargerThanEndHorizontal) {
		*startHorizontalLargerThanEndHorizontal = (start.horizontal > end.horizontal) ? true : false;
	}
	ret.vertical = start.vertical - end.vertical;
	if (startVerticalLargerThanEndVertical) {
		*startVerticalLargerThanEndVertical = (start.vertical > end.vertical) ? true : false;
	}

	return ret;
}

Vec2D abs_diff(Vec2D start, Vec2D end, bool* startHorizontalLargerThanEndHorizontal, bool* startVerticalLargerThanEndVertical) {
	Vec2D ret;
	if (start.horizontal > end.horizontal) {
		ret.horizontal = start.horizontal - end.horizontal;
		if (startHorizontalLargerThanEndHorizontal) {
			*startHorizontalLargerThanEndHorizontal = true;
		}
	}
	else {
		ret.horizontal = end.horizontal - start.horizontal;
		if (startHorizontalLargerThanEndHorizontal) {
			*startHorizontalLargerThanEndHorizontal = false;
		}
	}

	if (start.vertical > end.vertical) {
		ret.vertical = start.vertical - end.vertical;
		if (startVerticalLargerThanEndVertical) {
			*startVerticalLargerThanEndVertical = true;
		}
	}
	else {
		ret.vertical = end.vertical - start.vertical;
		if (startVerticalLargerThanEndVertical) {
			*startVerticalLargerThanEndVertical = false;
		}
	}

	return ret;
}

struct VentLine {
	Vec2D start;
	Vec2D end;
};

void printBoard(std::vector<std::vector<int>>* overlappingLines)
{
#if PRINT_DEBUG_STATEMENTS
	if (!overlappingLines) {
		printf("Board cannot be printed.");
		return;
	}

	printf("\n");
	for (int i = 0; i < overlappingLines->size(); i++) {
		for (int column = 0; column < (*overlappingLines)[i].size(); column++) {
			printf("%d ", (*overlappingLines)[i][column]);
		}
		printf("\n");
	}
#endif
}

// part 1: At how many points do at least two lines overlap?
int getPositionsWithAtLeastTwoOverlappingLines(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for vent lines
	std::string line;
	std::string startVector;
	std::string direction;
	std::string endVector;
	std::vector<VentLine> ventLines = std::vector<VentLine>();
	int maxHorizontalPos = 0;
	int maxVerticalPos = 0;
	int totalMatchingVentsForLargestOverlap = 0;
	const int overlapLineMininum = 2;

	while (std::getline(input, line))
	{
		VentLine ventLine;
		std::istringstream iss(line);
		if (!(iss >> startVector >> direction >> endVector)) {
			break; // error
		}

		// process input
		std::string delimiter = ",";
		ventLine.start.horizontal = std::stoi(startVector.substr(0, startVector.find(delimiter)), nullptr, 10);
		ventLine.start.vertical = std::stoi(startVector.substr(startVector.find(delimiter) + 1), nullptr, 10);
		ventLine.end.horizontal = std::stoi(endVector.substr(0, endVector.find(delimiter)), nullptr, 10);
		ventLine.end.vertical = std::stoi(endVector.substr(endVector.find(delimiter) + 1), nullptr, 10);

		ventLines.push_back(ventLine);

		// calc dimensions of vent "board"
		if (ventLine.start.horizontal > maxHorizontalPos) {
			maxHorizontalPos = ventLine.start.horizontal;
		}
		else if (ventLine.end.horizontal > maxVerticalPos) {
			maxHorizontalPos = ventLine.end.horizontal;
		}

		if (ventLine.start.vertical > maxHorizontalPos) {
			maxVerticalPos = ventLine.start.vertical;
		}
		else if (ventLine.end.vertical > maxVerticalPos) {
			maxVerticalPos = ventLine.end.vertical;
		}
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	// calculate overlapping lines
	std::vector<std::vector<int>> overlappingLines = std::vector<std::vector<int>>(maxHorizontalPos + 1, { std::vector<int>(maxVerticalPos + 1) });
	int largestOverlapNum = 0;

	for (int i = 0; i < ventLines.size(); i++) {
		bool isStartHorizontalLargerThanEndHorizontal = false;
		bool isStartVerticalLargerThanEndVertical = false;
		Vec2D diffVec = abs_diff(ventLines[i].start, ventLines[i].end, &isStartHorizontalLargerThanEndHorizontal, &isStartVerticalLargerThanEndVertical);
//#if PRINT_DEBUG_STATEMENTS
		printf("vecLine (start [%d, %d] end [%d, %d])\n", ventLines[i].start.horizontal, ventLines[i].start.vertical, ventLines[i].end.horizontal, ventLines[i].end.vertical);
//#endif
		// this puzzle only calculates where x1 == x2 or y1 == y2
		if (ventLines[i].start.horizontal == ventLines[i].end.horizontal) {
			for (int row = ((isStartVerticalLargerThanEndVertical) ? ventLines[i].end.vertical : ventLines[i].start.vertical); (isStartVerticalLargerThanEndVertical) ? row <= ventLines[i].start.vertical : row <= ventLines[i].end.vertical; row++) {
				// for some reason the board as per the example uses 'horizontal' as 'vertical' and 'vertical' as 'horizontal'
				overlappingLines[row][ventLines[i].start.horizontal] += 1;
#if PRINT_DEBUG_STATEMENTS
				printf("\tpos:[%d, %d]\n", ventLines[i].start.horizontal, row);
#endif
			}
		}

		if (ventLines[i].start.vertical == ventLines[i].end.vertical) { 
			// for(int column = 349; column <= 682; column++) 
			for (int column = ((isStartHorizontalLargerThanEndHorizontal) ? ventLines[i].end.horizontal : ventLines[i].start.horizontal); (isStartHorizontalLargerThanEndHorizontal) ? column <= ventLines[i].start.horizontal : column <= ventLines[i].end.horizontal; column++) {
				// for some reason the board as per the example uses 'horizontal' as 'vertical' and 'vertical' as 'horizontal'
				overlappingLines[ventLines[i].start.vertical][column] += 1;
#if PRINT_DEBUG_STATEMENTS
				printf("\tpos:[%d, %d]\n", column, ventLines[i].start.vertical);
#endif
			}
		}

		printBoard(&overlappingLines);
	}

	// print final vent board
//#if PRINT_DEBUG_STATEMENTS
	printf("\n");
	for (int i = 0; i < overlappingLines.size(); i++) {
		for (int column = 0; column < overlappingLines[i].size(); column++) {
			printf("%d ", overlappingLines[i][column]);
			if (overlappingLines[i][column] >= overlapLineMininum) {
				totalMatchingVentsForLargestOverlap++;
			}
		}
		printf("\n");
//#endif
	}

	return totalMatchingVentsForLargestOverlap;
}

// part 2: At how many points do at least two lines overlap (including diagonal)?
int getPositionsWithAtLeastTwoOverlappingLinesIncludingDiagonal(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for vent lines
	std::string line;
	std::string startVector;
	std::string direction;
	std::string endVector;
	std::vector<VentLine> ventLines = std::vector<VentLine>();
	int maxHorizontalPos = 0;
	int maxVerticalPos = 0;
	int totalMatchingVentsForLargestOverlap = 0;
	const int overlapLineMininum = 2;

	while (std::getline(input, line))
	{
		VentLine ventLine;
		std::istringstream iss(line);
		if (!(iss >> startVector >> direction >> endVector)) {
			break; // error
		}

		// process input
		std::string delimiter = ",";
		ventLine.start.horizontal = std::stoi(startVector.substr(0, startVector.find(delimiter)), nullptr, 10);
		ventLine.start.vertical = std::stoi(startVector.substr(startVector.find(delimiter) + 1), nullptr, 10);
		ventLine.end.horizontal = std::stoi(endVector.substr(0, endVector.find(delimiter)), nullptr, 10);
		ventLine.end.vertical = std::stoi(endVector.substr(endVector.find(delimiter) + 1), nullptr, 10);

		ventLines.push_back(ventLine);

		// calc dimensions of vent "board"
		if (ventLine.start.horizontal > maxHorizontalPos) {
			maxHorizontalPos = ventLine.start.horizontal;
		}
		else if (ventLine.end.horizontal > maxVerticalPos) {
			maxHorizontalPos = ventLine.end.horizontal;
		}

		if (ventLine.start.vertical > maxHorizontalPos) {
			maxVerticalPos = ventLine.start.vertical;
		}
		else if (ventLine.end.vertical > maxVerticalPos) {
			maxVerticalPos = ventLine.end.vertical;
		}
	}

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	// calculate overlapping lines
	std::vector<std::vector<int>> overlappingLines = std::vector<std::vector<int>>(maxHorizontalPos + 1, { std::vector<int>(maxHorizontalPos + 1) });
	int largestOverlapNum = 0;

	for (int i = 0; i < ventLines.size(); i++) {
		bool isStartHorizontalLargerThanEndHorizontal = false;
		bool isStartVerticalLargerThanEndVertical = false;
		Vec2D diffVec = abs_diff(ventLines[i].start, ventLines[i].end, &isStartHorizontalLargerThanEndHorizontal, &isStartVerticalLargerThanEndVertical);
//#if PRINT_DEBUG_STATEMENTS
		//printf("vecLine (start [%d, %d] end [%d, %d])\n", ventLines[i].start.horizontal, ventLines[i].start.vertical, ventLines[i].end.horizontal, ventLines[i].end.vertical);
//#endif
		// handle diagonal line

		if (isDiagonal(ventLines[i].start, ventLines[i].end)) {
			// An entry like 1, 1 -> 3, 3 covers points 1, 1, 2, 2, and 3, 3.
			if (ventLines[i].start.vertical == ventLines[i].start.horizontal && ventLines[i].end.vertical == ventLines[i].end.horizontal) {
				//printf("Diagonal line found. An entry like 1, 1 -> 3, 3 covers points 1, 1, 2, 2, and 3, 3.\n");
				for (int column = (isALargerthanB(ventLines[i].start.vertical, ventLines[i].end.vertical) ? ventLines[i].end.vertical : ventLines[i].start.vertical); 
					column <= (isALargerthanB(ventLines[i].start.vertical, ventLines[i].end.vertical) ? ventLines[i].start.vertical : ventLines[i].end.vertical); column++) {
//#if PRINT_DEBUG_STATEMENTS
					//printf("\tpos:[%d, %d]\n", column, column);
//#endif
					overlappingLines[column][column] += 1;
				}
				printBoard(&overlappingLines);
			}
			// An entry like 9, 7 -> 7, 9 covers points 9, 7, 8, 8, and 7, 9.
			else if (ventLines[i].start.vertical == ventLines[i].end.horizontal && ventLines[i].start.horizontal == ventLines[i].end.vertical) {
				//printf("Diagonal line found. An entry like 9, 7 -> 7, 9 covers points 9, 7, 8, 8, and 7, 9.\n");
				for (int abs_diff = 0; abs_diff <= abs(ventLines[i].start.horizontal - ventLines[i].end.horizontal); abs_diff++) {
					int column = (isALargerthanB(ventLines[i].start.horizontal, ventLines[i].end.horizontal) ? ventLines[i].end.horizontal : ventLines[i].start.horizontal);
					int row = (isALargerthanB(ventLines[i].start.vertical, ventLines[i].end.vertical) ? ventLines[i].start.vertical : ventLines[i].end.vertical);
//#if PRINT_DEBUG_STATEMENTS
					//printf("\tpos:[%d, %d]\n", column + abs_diff, row - abs_diff);
//#endif
					overlappingLines[row - abs_diff][column + abs_diff] += 1;
				}
				printBoard(&overlappingLines);
			}
			// An entry like 5, 5 -> 2, 8 covers points 5, 5, 4, 6, 3, 7, and 2, 8.
			else {
				//printf("!!! Diagonal line found. An entry like 5, 5 -> 2, 8 covers points 5, 5, 4, 6, 3, 7, and 2, 8. !!!\n");
				bool aHorizontalIsLarger = false;
				bool aVerticalIsLarger = false;
				Vec2D diff2d = diff(ventLines[i].start, ventLines[i].end, &aHorizontalIsLarger, &aVerticalIsLarger);
				if (diff2d.horizontal > 0 && diff2d.vertical > 0) {
					for (int horDiff = 0, verDiff = 0;
						aHorizontalIsLarger ? (horDiff <= diff2d.horizontal) : (horDiff > diff2d.horizontal);
						aHorizontalIsLarger ? horDiff++ : horDiff--,
						aVerticalIsLarger ? verDiff++ : verDiff--) {
						int column = aHorizontalIsLarger ? ventLines[i].start.horizontal - horDiff : ventLines[i].end.horizontal + horDiff;
						int row = aVerticalIsLarger ? ventLines[i].start.vertical - verDiff : ventLines[i].end.vertical + verDiff;

						//printf("\tpos:[%d, %d]\n", column, row);
						overlappingLines[row][column] += 1;
					}
				}
				else if (diff2d.horizontal > 0 && diff2d.vertical < 0) {
					for (int horDiff = 0, verDiff = 0;
						aHorizontalIsLarger ? (horDiff <= diff2d.horizontal) : (horDiff > diff2d.horizontal);
						aHorizontalIsLarger ? horDiff++ : horDiff--,
						aVerticalIsLarger ? verDiff++ : verDiff--) {
						int column = aHorizontalIsLarger ? ventLines[i].start.horizontal - horDiff : ventLines[i].end.horizontal + horDiff;
						int row = aVerticalIsLarger ? ventLines[i].end.vertical + verDiff : ventLines[i].start.vertical - verDiff;

						//printf("\tpos:[%d, %d]\n", column, row);
						try {
							overlappingLines[row][column] += 1;
						}
						catch (const char* message) {
							int m = 8;
						}
					}
				}
				else if (diff2d.horizontal < 0 && diff2d.vertical < 0) {
					for (int horDiff = 0, verDiff = 0;
						aHorizontalIsLarger ? (horDiff < diff2d.horizontal) : (horDiff >= diff2d.horizontal);
						aHorizontalIsLarger ? horDiff++ : horDiff--,
						aVerticalIsLarger ? verDiff++ : verDiff--) {
						int column = aHorizontalIsLarger ? ventLines[i].end.horizontal + horDiff : ventLines[i].start.horizontal - horDiff;
						int row = aVerticalIsLarger ? ventLines[i].end.vertical + verDiff : ventLines[i].start.vertical - verDiff;

						//printf("\tpos:[%d, %d]\n", column, row);
						overlappingLines[row][column] += 1;
					}
				}
				else if (diff2d.horizontal < 0 && diff2d.vertical > 0) {
					for (int horDiff = 0, verDiff = 0;
						aHorizontalIsLarger ? (horDiff < diff2d.horizontal) : (horDiff >= diff2d.horizontal);
						aHorizontalIsLarger ? horDiff++ : horDiff--,
						aVerticalIsLarger ? verDiff++ : verDiff--) {
						int column = aHorizontalIsLarger ? ventLines[i].start.horizontal - horDiff : ventLines[i].end.horizontal + horDiff;
						int row = aVerticalIsLarger ? ventLines[i].end.vertical + verDiff : ventLines[i].start.vertical - verDiff;

						//printf("\tpos:[%d, %d]\n", column, row);
						overlappingLines[row][column] += 1;
					}
				}
			}
		}
		else {
			if (ventLines[i].start.horizontal == ventLines[i].end.horizontal) {
				for (int row = ((isStartVerticalLargerThanEndVertical) ? ventLines[i].end.vertical : ventLines[i].start.vertical); (isStartVerticalLargerThanEndVertical) ? row <= ventLines[i].start.vertical : row <= ventLines[i].end.vertical; row++) {
					// for some reason the board as per the example uses 'horizontal' as 'vertical' and 'vertical' as 'horizontal'
					overlappingLines[row][ventLines[i].start.horizontal] += 1;
	#if PRINT_DEBUG_STATEMENTS
					printf("\tpos:[%d, %d]\n", ventLines[i].start.horizontal, row);
	#endif
				}
				printBoard(&overlappingLines);
			}

			if (ventLines[i].start.vertical == ventLines[i].end.vertical) {
				// for(int column = 349; column <= 682; column++) 
				for (int column = ((isStartHorizontalLargerThanEndHorizontal) ? ventLines[i].end.horizontal : ventLines[i].start.horizontal); (isStartHorizontalLargerThanEndHorizontal) ? column <= ventLines[i].start.horizontal : column <= ventLines[i].end.horizontal; column++) {
					// for some reason the board as per the example uses 'horizontal' as 'vertical' and 'vertical' as 'horizontal'
					overlappingLines[ventLines[i].start.vertical][column] += 1;
	#if PRINT_DEBUG_STATEMENTS
					printf("\tpos:[%d, %d]\n", column, ventLines[i].start.vertical);
	#endif
				}
				printBoard(&overlappingLines);
			}
		}
	}

	printBoard(&overlappingLines);

	// print final vent board
//#if PRINT_DEBUG_STATEMENTS
	printf("\n");
	for (int i = 0; i < overlappingLines.size(); i++) {
		for (int column = 0; column < overlappingLines[i].size(); column++) {
			printf("%d ", overlappingLines[i][column]);
			if (overlappingLines[i][column] >= overlapLineMininum) {
				totalMatchingVentsForLargestOverlap++;
			}
		}
		printf("\n");
		//#endif
	}

	return totalMatchingVentsForLargestOverlap;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\Day 5 - Hydrothermal Adventure\\vents.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/5
	int answer1 = getPositionsWithAtLeastTwoOverlappingLines(fpath);
	int answer2 = getPositionsWithAtLeastTwoOverlappingLinesIncludingDiagonal(fpath);

	std::cout << "At how many points do at least two lines overlap?: " << answer1 << std::endl << std::endl;
	std::cout << "At how many points do at least two lines overlap (including diagonal)?: " << answer2 << std::endl << std::endl;
}