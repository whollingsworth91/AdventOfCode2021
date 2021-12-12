#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#define PRINT_DEBUG_STATEMENTS 0

typedef int bingoNumber;
typedef bool isMatchingNumber;
typedef bool isWinningBoard;

#define arrayIntBoolPair std::vector<std::pair<bingoNumber, isMatchingNumber>>
typedef arrayIntBoolPair BingoRow;

#define bingoBoardWinState std::pair<std::vector<BingoRow>, isWinningBoard>
typedef bingoBoardWinState BingoBoardWithWinState;

// part 1: What will your final score be if you choose that board?
int getWhichBoardWillWin(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for bingo numbers and boards
	std::string line;
	std::string bingoLine;

	// read the bingo number line
	std::string bingoSequence;
	std::getline(input, bingoSequence);

	// skip first space line
	std::string skipLine;
	std::getline(input, skipLine);
	
	// now parse the bingo boards from file
	std::vector<BingoBoardWithWinState> bingoBoards = std::vector<BingoBoardWithWinState>();
	BingoBoardWithWinState currentBingoBoard = BingoBoardWithWinState();

	while (std::getline(input, line))
	{
		if(line.compare("") != 0) {
			// read bingo line and add it to the bingo board
			std::istringstream iss(line);
			BingoRow bingoRow = BingoRow(5, { 0, false }); // initialize all booleans to false - they will be used to check winning rows/columns
			if (!(iss >> bingoRow[0].first >> bingoRow[1].first >> bingoRow[2].first >> bingoRow[3].first >> bingoRow[4].first)) {
				break; // error
			}
			currentBingoBoard.first.push_back(bingoRow);
			currentBingoBoard.second = false;
		}
		else {
			// end of current board, add it to our list of boards and reset info for next board
			bingoBoards.push_back(currentBingoBoard);
			currentBingoBoard.first.clear();
		}
	}

	// push the last board in
	bingoBoards.push_back(currentBingoBoard);
	currentBingoBoard.first.clear();

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	// "call out" the bingo numbers
	int winningBoardIndex = -1;
	std::string delimiter = ",";
	size_t pos = 0;
	int bingoNumber = 0;

	while ((pos = bingoSequence.find(delimiter)) != std::string::npos && winningBoardIndex == -1) {
		bingoNumber = std::stoi(bingoSequence.substr(0, pos), nullptr, 10);
		bingoSequence.erase(0, pos + delimiter.length());

		// check all boards for a winning row or column
		for (int i = 0; i < bingoBoards.size(); i++) {
			for (int row = 0; row < bingoBoards[i].first.size(); row++) {
				for (int column = 0; column < bingoBoards[i].first[row].size(); column++) {
					if (bingoNumber == bingoBoards[i].first[row][column].first) {
						bingoBoards[i].first[row][column].second = true;
					}

					// check if any row or column is a winner
					if (bingoBoards[i].first[row][0].second && bingoBoards[i].first[row][1].second && bingoBoards[i].first[row][2].second && bingoBoards[i].first[row][3].second && bingoBoards[i].first[row][4].second) {
						winningBoardIndex = i;
						break;
					}
					else if (bingoBoards[i].first[0][column].second && bingoBoards[i].first[1][column].second && bingoBoards[i].first[2][column].second && bingoBoards[i].first[3][column].second && bingoBoards[i].first[4][column].second) {
						winningBoardIndex = i;
						break;
					}
				}
			}
		}
	}

	// calculate winning board's sum
	int sumOfUnmarkedNumbers = 0;
	for (int row = 0; row < bingoBoards[winningBoardIndex].first.size(); row++) {
		for (int column = 0; column < bingoBoards[winningBoardIndex].first[row].size(); column++) {
			if (!bingoBoards[winningBoardIndex].first[row][column].second) {
				sumOfUnmarkedNumbers += bingoBoards[winningBoardIndex].first[row][column].first;
			}
		}
	}

	return sumOfUnmarkedNumbers * bingoNumber;
}

// part 2: Once it wins, what would its final score be?
int getWhichBoardWinsLast(std::string fpath)
{
	// open file for parsing
	std::ifstream input;
	input.open(fpath, std::ios::in);

	if (!input) {
		std::cout << std::endl << "Could not read the file" << std::endl;
	}

	// parse file for bingo numbers and boards
	std::string line;
	std::string bingoLine;

	// read the bingo number line
	std::string bingoSequence;
	std::getline(input, bingoSequence);

	// skip first space line
	std::string skipLine;
	std::getline(input, skipLine);

	// now parse the bingo boards from file
	std::vector<BingoBoardWithWinState> bingoBoards = std::vector<BingoBoardWithWinState>();
	BingoBoardWithWinState currentBingoBoard = BingoBoardWithWinState();

	while (std::getline(input, line))
	{
		if (line.compare("") != 0) {
			// read bingo line and add it to the bingo board
			std::istringstream iss(line);
			BingoRow bingoRow = BingoRow(5, { 0, false }); // initialize all booleans to false - they will be used to check winning rows/columns
			if (!(iss >> bingoRow[0].first >> bingoRow[1].first >> bingoRow[2].first >> bingoRow[3].first >> bingoRow[4].first)) {
				break; // error
			}
			currentBingoBoard.first.push_back(bingoRow);
			currentBingoBoard.second = false;
		}
		else {
			// end of current board, add it to our list of boards and reset info for next board
			bingoBoards.push_back(currentBingoBoard);
			currentBingoBoard.first.clear();
		}
	}

	// push the last board in
	bingoBoards.push_back(currentBingoBoard);
	currentBingoBoard.first.clear();

	//closing the file after reading
	input.close();
	std::cout << std::endl;

	// "call out" the bingo numbers
	int winningBoardsCount = 0;
	int lastWinningBoardIndex = -1;
	std::string delimiter = ",";
	size_t pos = 0;
	int bingoNumber = 0;

	while ((pos = bingoSequence.find(delimiter)) != std::string::npos && winningBoardsCount < bingoBoards.size()) {
		bingoNumber = std::stoi(bingoSequence.substr(0, pos), nullptr, 10);
		bingoSequence.erase(0, pos + delimiter.length());

		// check all boards for a winning row or column
		for (int i = 0; i < bingoBoards.size(); i++) {
			if(bingoBoards[i].second == false) {
				bool winningBoard = false;
				for (int row = 0; row < bingoBoards[i].first.size(); row++) {
					for (int column = 0; column < bingoBoards[i].first[row].size(); column++) {
						if (bingoNumber == bingoBoards[i].first[row][column].first) {
							bingoBoards[i].first[row][column].second = true;
						}

						// check if any row is a winner and set the bingo board as a winning board
						if (bingoBoards[i].first[row][0].second && bingoBoards[i].first[row][1].second && bingoBoards[i].first[row][2].second && bingoBoards[i].first[row][3].second && bingoBoards[i].first[row][4].second) {
							bingoBoards[i].second = true;
							winningBoard = true;
							break;
						}
						else if (bingoBoards[i].first[0][column].second && bingoBoards[i].first[1][column].second && bingoBoards[i].first[2][column].second && bingoBoards[i].first[3][column].second && bingoBoards[i].first[4][column].second) {
							bingoBoards[i].second = true;
							winningBoard = true;
							break;
						}
					}
				}
				
				if (winningBoard) {
					lastWinningBoardIndex = i;
					winningBoardsCount++;
				}
			}
		}
	}

	// calculate winning board's sum
	int sumOfUnmarkedNumbers = 0;
	for (int row = 0; row < bingoBoards[lastWinningBoardIndex].first.size(); row++) {
		for (int column = 0; column < bingoBoards[lastWinningBoardIndex].first[row].size(); column++) {
			if (!bingoBoards[lastWinningBoardIndex].first[row][column].second) {
				sumOfUnmarkedNumbers += bingoBoards[lastWinningBoardIndex].first[row][column].first;
			}
		}
	}

	return sumOfUnmarkedNumbers* bingoNumber;
}

int main(int argc, char* argv[])
{
	// init file path
	std::cout << argv[0];
	std::string fpath = argv[0];
	fpath.append("\\..\\..\\Day 4 - Giant Squid\\bingo.txt");

	// get answers for part 1 & 2
	// https://adventofcode.com/2021/day/4
	int answer1 = getWhichBoardWillWin(fpath);
	int answer2 = getWhichBoardWinsLast(fpath);

	std::cout << "What will your final score be if you choose that board?: " << answer1 << std::endl << std::endl;
	std::cout << "Once it wins, what would its final score be?: " << answer2 << std::endl << std::endl;

	std::string read;
	std::cin >> read;
}