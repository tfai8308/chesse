#include "Queen.h"

Queen::Queen(bool color) {
	if (color) {
		pieceType.setTexture(TextureManager::GetTexture("white_queen"));
	}
	else {
		pieceType.setTexture(TextureManager::GetTexture("black_queen"));
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
	name = "Quee";
}

void Queen::CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	//Moves used for playing the game
	legalMoves.clear();
	legalMovesIncludingFriendlyFire.clear();
	lineOfSight = true; //Send a LoS check all the way in one direction, false if hits friendly or wall
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

	lineOfSight = true; //Send a LoS check all the way in one direction, false if hits friendly or wall
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, -pivot, activeColIndex, pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up-Right
	}

	lineOfSight = true; //Reset the LoS check for the other diagonals
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, -pivot, activeColIndex, -pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up-Left
	}

	lineOfSight = true; //So on
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, pivot, activeColIndex, pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Right
	}

	lineOfSight = true; //And so forth
	for (int pivot = 1; lineOfSight; pivot++) {
		AddMove(gridPieces, activeRowIndex, pivot, activeColIndex, -pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Left
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

	lineOfSight = true; //Send a LoS check all the way in one direction, false if hits friendly or wall
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, -pivot, activeColIndex, pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up-Right
	}

	lineOfSight = true; //Reset the LoS check for the other diagonals
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, -pivot, activeColIndex, -pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Up-Left
	}

	lineOfSight = true; //So on
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, pivot, activeColIndex, pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Right
	}

	lineOfSight = true; //And so forth
	for (int pivot = 1; lineOfSight; pivot++) {
		AddTotalMove(gridPieces, activeRowIndex, pivot, activeColIndex, -pivot, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Left
	}
}