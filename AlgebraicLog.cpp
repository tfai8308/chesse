#include "AlgebraicLog.h"

int AlgebraicLog::logNumber = 0;

AlgebraicLog::AlgebraicLog() : Log() {

}

AlgebraicLog::AlgebraicLog(string name, int startRow, int startCol, int endRow, int endCol, bool isCapture, bool isPromotion, int promotionType, bool isCastle, int castleType, bool isEnPassant) {
	pieceName = name;
	this->startRow = startRow;
	this->startCol = startCol;
	this->endRow = endRow;
	this->endCol = endCol;
	logNumber++;

	string rowConvTable[] = { "8", "7", "6", "5", "4", "3", "2", "1" };
	string colConvTable[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

	string temp = "";
	temp += colConvTable[startCol];
	temp += rowConvTable[startRow];
	startPos = temp;

	temp = "";
	temp += colConvTable[endCol];
	temp += rowConvTable[endRow];
	endPos = temp;

	logMessage = to_string(logNumber) + ". ";

	if (isCastle) {
		if (castleType == 0) {
			logMessage.append("0-0");
		}
		if (castleType == 1) {
			logMessage.append("0-0-0");
		}
	}
	else if (isPromotion) {
		string promotedTo[] = { "=N", "=B", "=R", "=Q" };
		logMessage.append(endPos + promotedTo[promotionType]);
	}
	else {
		if (pieceName == "Pawn") {
			if (isCapture) {
				logMessage.append(startPos.substr(0, 1) + 'x' + endPos);
				if (isEnPassant) {
					logMessage.append(" e.p.");
				}
			}
			else {
				logMessage.append(startPos + endPos);
			}
		}
		else {
			if (isCapture) {
				logMessage.append(pieceName.substr(0, 1) + startPos + 'x' + endPos);
			}
			else {
				logMessage.append(pieceName.substr(0, 1) + startPos + endPos);
			}

		}
	}
}

void AlgebraicLog::AppendCheckSymbol() {
	logMessage.append("+");
}

void AlgebraicLog::AppendDrawSymbol() {
	logMessage.append("\n");
	logMessage.append("1/2 - 1/2");
}

void AlgebraicLog::AppendVictorySymbol(bool color) {
	logMessage.erase(logMessage.begin() + logMessage.size() - 1);
	logMessage.append("#");
	logMessage.append("\n");
	
	if (color) {
		logMessage.append("1-0");
	}
	else {
		logMessage.append("0-1");
	}
}

void AlgebraicLog::PrintLog() {
	cout << logMessage << endl;
}