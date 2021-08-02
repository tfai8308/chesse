#include "Log.h"

Log::Log() {
	pieceName = "none";
	startPos = "none";
	startRow, startCol = -1;
	endPos = "none";
	endRow, endCol = -1;
}

Log::Log(string name, int startRow, int startCol, int endRow, int endCol) {
	pieceName = name;
	this->startRow = startRow;
	this->startCol = startCol;
	this->endRow = endRow;
	this->endCol = endCol;

	string rowConvTable[] = {"8", "7", "6", "5", "4", "3", "2", "1"};
	string colConvTable[] = {"a", "b", "c", "d", "e", "f", "g", "h"};

	string temp = "";
	temp += colConvTable[startCol];
	temp += rowConvTable[startRow];
	startPos = temp;

	temp = "";
	temp += colConvTable[endCol];
	temp += rowConvTable[endRow];
	endPos = temp;

	logMessage = pieceName + " moved from " + startPos + " to " + endPos;
}

string Log::GetPieceName() {
	return pieceName;
}

string Log::GetStartCoord() {
	return startPos;
}

int Log::GetStartCoordRow() {
	return startRow;
}

int Log::GetStartCoordCol() {
	return startCol;
}

string Log::GetEndCoord() {
	return endPos;
}

int Log::GetEndCoordRow() {
	return endRow;
}

int Log::GetEndCoordCol() {
	return endCol;
}

void Log::PrintLog() {
	cout << logMessage << endl;
}

bool Log::operator==(const Log& rhs) const {
	return 	this->pieceName == rhs.pieceName &&
	this->startRow == rhs.startRow &&
	this->startCol == rhs.startCol &&
	this->endRow == rhs.endRow &&
	this->endCol == rhs.endCol;
}