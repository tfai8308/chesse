#pragma once
#include "ChessPiece.h"

class King : public ChessPiece {
public:
	King(bool color);
	void CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces);
	void AddKingMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces);
	void CheckCanCastle(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces);
};