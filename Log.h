#pragma once
#include <iostream>
#include <string>

using namespace std;

class Log {
private:
	string logMessage;

protected:
	string pieceName;
	string startPos;
	int startRow;
	int startCol;
	string endPos;
	int endRow;
	int endCol;

public:
	Log();
	Log(string name, int startRow, int startCol, int endRow, int endCol);

	string GetPieceName();
	string GetStartCoord();
	int GetStartCoordRow();
	int GetStartCoordCol();
	string GetEndCoord();
	int GetEndCoordRow();
	int GetEndCoordCol();

	void PrintLog();

	bool operator==(const Log& rhs) const;
};

