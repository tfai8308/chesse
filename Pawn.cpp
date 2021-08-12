#include "Pawn.h"

Pawn::Pawn(bool color) {
	if (color) {
		pieceType.setTexture(TextureManager::GetTexture("white_pawn"));
	}
	else {
		pieceType.setTexture(TextureManager::GetTexture("black_pawn"));
	}
	ChessPiece::color = color;
	dragging = false;
	moveCount = 0;
	lineOfSight = true;
	canEnPassant = false;
	canCastle = false;
	canCastleLeft = false;
	canCastleRight = false;
	inCheck = false;
	attackingKing = false;
	protectingKing = false;
	alreadyModified = false;
	name = "Pawn";
}

void Pawn::AddPawnMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY, bool pawnDiagonal) {
	legalMoves.push_back(sf::Vector2f(activePieceOriginalPositionX + (96 * colOffset), activePieceOriginalPositionY + (96 * rowOffset)));
	legalMovesIncludingFriendlyFire.push_back(legalMoves[legalMoves.size() - 1]);
	try {
		ChessPiece* target = gridPieces.at(activeRowIndex + rowOffset).at(activeColIndex + colOffset);
		if (pawnDiagonal) { //Special Case for diagonal captures
			if (target == nullptr) {
				legalMoves.pop_back();
				legalMovesIncludingFriendlyFire.pop_back();
				return;
			}
			if (target->GetColor() == this->GetColor()) {
				legalMoves.pop_back();
				return;
			}
		}
		else {
			if (target != nullptr) {
				legalMoves.pop_back();
				legalMovesIncludingFriendlyFire.pop_back();
				lineOfSight = false;
				return;
			}
		}
	}
	catch (const out_of_range& e) {
		legalMoves.pop_back(); //The space wasn't on the board
		legalMovesIncludingFriendlyFire.pop_back();
		return;
	}
}

void Pawn::CheckEnPassant(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs) {
	if (color) {
		//White Checks Left and Right
		//E.P. Left
		try {
			ChessPiece* target = gridPieces.at(activeRowIndex).at(activeColIndex - 1);
			if (target != nullptr && target->GetColor() != this->GetColor() && target->GetName() == "Pawn" && target->GetMoveCount() == 1) {
				if (logs[logs.size() - 1].GetEndCoordCol() == (activeColIndex - 1)) {
					AddMove(gridPieces, activeRowIndex, -1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY);
					AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false);
					canEnPassant = true;
				}
			}
		}
		catch (const out_of_range& e) {
			//cout << e.what() << endl;
		}

		//E.P. Right
		try {
			ChessPiece* target = gridPieces.at(activeRowIndex).at(activeColIndex + 1);
			if (target != nullptr && target->GetColor() != this->GetColor() && target->GetName() == "Pawn" && target->GetMoveCount() == 1) {
				if (logs[logs.size() - 1].GetEndCoordCol() == (activeColIndex + 1)) {
					AddMove(gridPieces, activeRowIndex, -1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Left
					AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false); //Forward
					canEnPassant = true;
				}
			}
		}
		catch (const out_of_range& e) {
			//cout << e.what() << endl;
		}
	}
	else {
		//Black Checks Left and Right
		//E.P. Left
		try {
			ChessPiece* target = gridPieces.at(activeRowIndex).at(activeColIndex - 1);
			if (target != nullptr && target->GetColor() != this->GetColor() && target->GetName() == "Pawn" && target->GetMoveCount() == 1) {
				if (logs[logs.size() - 1].GetEndCoordCol() == (activeColIndex - 1)) {
					AddMove(gridPieces, activeRowIndex, 1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Left
					AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false); //Forward
					canEnPassant = true;
				}
			}
		}
		catch (const out_of_range& e) {
			//cout << e.what() << endl;
		}

		//E.P. Right
		try {
			ChessPiece* target = gridPieces.at(activeRowIndex).at(activeColIndex + 1);
			if (target != nullptr && target->GetColor() != this->GetColor() && target->GetName() == "Pawn" && target->GetMoveCount() == 1) {
				if (logs[logs.size() - 1].GetEndCoordCol() == (activeColIndex + 1)) {
					AddMove(gridPieces, activeRowIndex, 1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY); //Down-Left
					AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false); //Forward
					canEnPassant = true;
				}
			}
		}
		catch (const out_of_range& e) {
			//cout << e.what() << endl;
		}
	}
}

void Pawn::CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	legalMoves.clear();
	legalMovesIncludingFriendlyFire.clear();
	totalMoves.clear();

	lineOfSight = true; //Send a LoS check all the way in one direction, false if hits friendly or wall
	canEnPassant = false; //Assume nothing can en passant first
	//White pawn's moveset
	if (color) {
		if (moveCount == 0 && activeRowIndex == 6) { //First move
			for (int pivot = -1; pivot >= -2 && lineOfSight; pivot--) {
				AddPawnMove(gridPieces, activeRowIndex, pivot, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false);
			}
			AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
			AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
		}
		else if (activeRowIndex != 0) { //There won't be any valid moves on the 8th rank for a white pawn besides promote
			if (activeRowIndex == 3) {
				CheckEnPassant(gridPieces, activeRowIndex, activeColIndex, activePieceOriginalPositionX, activePieceOriginalPositionY, logs);
			} 
			if (!canEnPassant) {
				AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false);
				AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
				AddPawnMove(gridPieces, activeRowIndex, -1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
			}
		}
	}
	//Black pawn's moveset
	else {
		if (moveCount == 0 && activeRowIndex == 1) { //First move
			for (int pivot = 1; pivot <= 2 && lineOfSight; pivot++) {
				AddPawnMove(gridPieces, activeRowIndex, pivot, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false);
			}
			AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
			AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
		}
		else if (activeRowIndex != 7) { //There won't be any valid moves on the 1st rank for a black pawn besides promote
			if (activeRowIndex == 4) {
				CheckEnPassant(gridPieces, activeRowIndex, activeColIndex, activePieceOriginalPositionX, activePieceOriginalPositionY, logs);
			}
			if(!canEnPassant) {
				AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, false);
				AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
				AddPawnMove(gridPieces, activeRowIndex, 1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, true);
			}
		}
	}

	totalMoves = legalMoves;
}