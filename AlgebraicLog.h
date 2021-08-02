#pragma once
#include "Log.h"

class AlgebraicLog : public Log {
private:
	static int logNumber;
	string logMessage;

public:
	AlgebraicLog();
	AlgebraicLog(string name, int startRow, int startCol, int endRow, int endCol, bool isCapture, bool isPromotion, int promotionType, bool isCastle, int castleType, bool isEnPassant);
	
	void AppendCheckSymbol();
	void AppendDrawSymbol();
	void AppendVictorySymbol(bool color);

	void PrintLog();
};

