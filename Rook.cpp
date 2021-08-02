#include "Rook.h"

Rook::Rook(bool color) {
	if (color) {
		pieceType.setTexture(TextureManager::GetTexture("white_rook"));
	}
	else {
		pieceType.setTexture(TextureManager::GetTexture("black_rook"));
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
	name = "Rook";
}

void Rook::CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	//Moves used for playing the game
	legalMoves.clear();
	legalMovesIncludingFriendlyFire.clear();

	lineOfSight = true; //Send an LoS check all the way in one direction, false if hits friendly or wall
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, -pivot, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up
	}

	lineOfSight = true; //Reset the LoS check for the other straights
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, pivot, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down
	}

	lineOfSight = true; //So on
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, 0, activeColIndex, -pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Left
	}

	lineOfSight = true; //And so forth
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, 0, activeColIndex, pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Right
	}

	//Moves used for monitoring state
	totalMoves.clear();
	lineOfSight = true; //Send an LoS check all the way in one direction, false if hits wall
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, -pivot, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up
	}

	lineOfSight = true; //Reset the LoS check for the other straights
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, pivot, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down
	}

	lineOfSight = true; //So on
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, 0, activeColIndex, -pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Left
	}

	lineOfSight = true; //And so forth
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, 0, activeColIndex, pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Right
	}
}