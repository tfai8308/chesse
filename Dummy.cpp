#include "Dummy.h"

Dummy::Dummy(bool color) {
	pieceType.setTexture(TextureManager::GetTexture("debug"));
	ChessPiece::color = color;
	dragging = false;
	moveCount = 0;
	canEnPassant = false;
	canCastle = false;
	canCastleLeft = false;
	canCastleRight = false;
	inCheck = false;
	attackingKing = false;
	protectingKing = false;
	alreadyModified = false;
	name = "Dumm";
}

void Dummy::CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	//Has no moves
}