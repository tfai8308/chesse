#pragma once
#include "ChessPiece.h"

class Knight : public ChessPiece {
public:
	Knight(bool color);
	void CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces);
};

