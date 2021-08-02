#include "Knight.h"

Knight::Knight(bool color) {
	if (color) {
		pieceType.setTexture(TextureManager::GetTexture("white_knight"));
	}
	else {
		pieceType.setTexture(TextureManager::GetTexture("black_knight"));
	}
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
	name = "Nnig"; //For "Nnight"
}

void Knight::CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	legalMoves.clear();
	legalMovesIncludingFriendlyFire.clear();
	AddMove(gridPieces, activeRowIndex, -2, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY);  //Up-Right
	AddMove(gridPieces, activeRowIndex, -2, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up-Left
	AddMove(gridPieces, activeRowIndex, 1, activeColIndex, -2, activePieceOriginalPositionX, activePieceOriginalPositionY);  //Left-Down
	AddMove(gridPieces, activeRowIndex, -1, activeColIndex, -2, activePieceOriginalPositionX, activePieceOriginalPositionY); //Left-Up
	AddMove(gridPieces, activeRowIndex, 2, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY);   //Down-Right
	AddMove(gridPieces, activeRowIndex, 2, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY);  //Down-Left
	AddMove(gridPieces, activeRowIndex, 1, activeColIndex, 2, activePieceOriginalPositionX, activePieceOriginalPositionY);   //Right-Down
	AddMove(gridPieces, activeRowIndex, -1, activeColIndex, 2, activePieceOriginalPositionX, activePieceOriginalPositionY);  //Right-Up

	totalMoves = legalMoves;
}