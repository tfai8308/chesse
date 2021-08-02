#include "King.h"

King::King(bool color) {
	if (color) {
		pieceType.setTexture(TextureManager::GetTexture("white_king"));
	}
	else {
		pieceType.setTexture(TextureManager::GetTexture("black_king"));
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
	name = "King";
}

void King::AddKingMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	legalMoves.push_back(sf::Vector2f(activePieceOriginalPositionX + (96 * colOffset), activePieceOriginalPositionY + (96 * rowOffset)));
	legalMovesIncludingFriendlyFire.push_back(legalMoves[legalMoves.size() - 1]);

	try {
		ChessPiece* target = gridPieces.at(activeRowIndex + rowOffset).at(activeColIndex + colOffset);
		if (target != nullptr) { //Can't attack friendlies
			if (target->GetColor() == this->GetColor()) {
				legalMoves.pop_back();
				return;
			}
			else { //Can only capture if it doesn't kill the king
				if (color) {
					for (int i = 0; i < blackPieces.size(); i++) {
						if (blackPieces[i]->GetName() == "Pawn") { //The pawn is protected and cannot be captured safely
							if ((legalMoves[legalMoves.size() - 1].x == blackPieces[i]->GetSprite().getPosition().x + 96 &&
								legalMoves[legalMoves.size() - 1].y == blackPieces[i]->GetSprite().getPosition().y + 96) ||
								(legalMoves[legalMoves.size() - 1].x == blackPieces[i]->GetSprite().getPosition().x - 96 &&
								legalMoves[legalMoves.size() - 1].y == blackPieces[i]->GetSprite().getPosition().y + 96)) {
								legalMoves.pop_back();
								legalMovesIncludingFriendlyFire.pop_back();
								return;
							}
						}
						else {
							for (int j = 0; j < blackPieces[i]->GetLegalMovesWithFF().size(); j++) {
								if (legalMoves[legalMoves.size() - 1] == blackPieces[i]->GetLegalMovesWithFF()[j]) { //The piece is protected and cannot be captured safely
									legalMoves.pop_back();
									legalMovesIncludingFriendlyFire.pop_back();
									return;
								}
							}
						}
					}
				}
				else {
					for (int i = 0; i < whitePieces.size(); i++) {
						if (whitePieces[i]->GetName() == "Pawn") { //The pawn is protected and cannot be captured safely
							if ((legalMoves[legalMoves.size() - 1].x == whitePieces[i]->GetSprite().getPosition().x + 96 &&
								legalMoves[legalMoves.size() - 1].y == whitePieces[i]->GetSprite().getPosition().y - 96) ||
								(legalMoves[legalMoves.size() - 1].x == whitePieces[i]->GetSprite().getPosition().x - 96 &&
								legalMoves[legalMoves.size() - 1].y == whitePieces[i]->GetSprite().getPosition().y - 96)) {
								legalMoves.pop_back();
								legalMovesIncludingFriendlyFire.pop_back();
								return;
							}
						}
						else {
							for (int j = 0; j < whitePieces[i]->GetLegalMovesWithFF().size(); j++) {
								if (legalMoves[legalMoves.size() - 1] == whitePieces[i]->GetLegalMovesWithFF()[j]) { //The piece is protected and cannot be captured safely
									legalMoves.pop_back();
									legalMovesIncludingFriendlyFire.pop_back();
									return;
								}
							}
						}
					}
				}
			}
		}
		else { //Can't move onto enemy controlled tiles
			if (inCheck) {
				if (color) { //A White King in check must move to a safe square
					for (int i = 0; i < blackPieces.size(); i++) {
						if (blackPieces[i]->IsAttackingKing()) {
							if (blackPieces[i]->GetName() == "Pawn") {
								//If a pawn is checking the king, the king may still move anywhere else
							}
							else {
								for (int j = 0; j < blackPieces[i]->GetTotalMoves().size(); j++) { //Can't stay in enemy space
									if (legalMoves[legalMoves.size() - 1] == blackPieces[i]->GetTotalMoves()[j]) {
										legalMoves.pop_back();
										legalMovesIncludingFriendlyFire.pop_back();
										return;
									}
								}
							}
						}
						else {
							if (blackPieces[i]->GetName() == "Pawn") {
								if ((legalMoves[legalMoves.size() - 1].x == blackPieces[i]->GetSprite().getPosition().x + 96 &&
									legalMoves[legalMoves.size() - 1].y == blackPieces[i]->GetSprite().getPosition().y + 96) ||
									(legalMoves[legalMoves.size() - 1].x == blackPieces[i]->GetSprite().getPosition().x - 96 &&
									legalMoves[legalMoves.size() - 1].y == blackPieces[i]->GetSprite().getPosition().y + 96)) {
									legalMoves.pop_back();
									legalMovesIncludingFriendlyFire.pop_back();
									return;
								}
							}
							else {
								for (int j = 0; j < blackPieces[i]->GetLegalMoves().size(); j++) { //Can't move into enemy space
									if (legalMoves[legalMoves.size() - 1] == blackPieces[i]->GetLegalMoves()[j]) {
										legalMoves.pop_back();
										legalMovesIncludingFriendlyFire.pop_back();
										return;
									}
								}
							}
						}
					}
				}
				else { //A Black King in check must move to a safe square
					for (int i = 0; i < whitePieces.size(); i++) {
						if (whitePieces[i]->IsAttackingKing()) {
							if (whitePieces[i]->GetName() == "Pawn") {
								//If a pawn is checking the king, the king may still move anywhere else
							}
							else {
								for (int j = 0; j < whitePieces[i]->GetTotalMoves().size(); j++) { //Can't stay in enemy space
									if (legalMoves[legalMoves.size() - 1] == whitePieces[i]->GetTotalMoves()[j]) {
										legalMoves.pop_back();
										legalMovesIncludingFriendlyFire.pop_back();
										return;
									}
								}
							}
						}
						else {
							if (whitePieces[i]->GetName() == "Pawn") {
								if ((legalMoves[legalMoves.size() - 1].x == whitePieces[i]->GetSprite().getPosition().x + 96 &&
									legalMoves[legalMoves.size() - 1].y == whitePieces[i]->GetSprite().getPosition().y - 96) ||
									(legalMoves[legalMoves.size() - 1].x == whitePieces[i]->GetSprite().getPosition().x - 96 &&
									legalMoves[legalMoves.size() - 1].y == whitePieces[i]->GetSprite().getPosition().y - 96)) {
									legalMoves.pop_back();
									legalMovesIncludingFriendlyFire.pop_back();
									return;
								}
							}
							else {
								for (int j = 0; j < whitePieces[i]->GetLegalMoves().size(); j++) { //Can't move into enemy space
									if (legalMoves[legalMoves.size() - 1] == whitePieces[i]->GetLegalMoves()[j]) {
										legalMoves.pop_back();
										legalMovesIncludingFriendlyFire.pop_back();
										return;
									}
								}
							}
						}
					}
				}
			}
			else {
				if (color) { //White King cannot move into a Black controlled tile
					for (int i = 0; i < blackPieces.size(); i++) {
						if (blackPieces[i]->GetName() == "Pawn") { //The King can't move diagonal to a pawn
							if ((legalMoves[legalMoves.size() - 1].x == blackPieces[i]->GetSprite().getPosition().x + 96 &&
								legalMoves[legalMoves.size() - 1].y == blackPieces[i]->GetSprite().getPosition().y + 96) ||
								(legalMoves[legalMoves.size() - 1].x == blackPieces[i]->GetSprite().getPosition().x - 96 &&
								legalMoves[legalMoves.size() - 1].y == blackPieces[i]->GetSprite().getPosition().y + 96)) {
								legalMoves.pop_back();
								legalMovesIncludingFriendlyFire.pop_back();
								return;
							}
						}
						else {
							for (int j = 0; j < blackPieces[i]->GetLegalMoves().size(); j++) { //Can't move into enemy space
								if (legalMoves[legalMoves.size() - 1] == blackPieces[i]->GetLegalMoves()[j]) {
									legalMoves.pop_back();
									legalMovesIncludingFriendlyFire.pop_back();
									return;
								}
							}
						}
					}
				}
				else { //Black King cannot move into a White controlled tile
					for (int i = 0; i < whitePieces.size(); i++) {
						if (whitePieces[i]->GetName() == "Pawn") { //The King can't move diagonal to a pawn
							if ((legalMoves[legalMoves.size() - 1].x == whitePieces[i]->GetSprite().getPosition().x + 96 &&
								legalMoves[legalMoves.size() - 1].y == whitePieces[i]->GetSprite().getPosition().y - 96) ||
								(legalMoves[legalMoves.size() - 1].x == whitePieces[i]->GetSprite().getPosition().x - 96 &&
								legalMoves[legalMoves.size() - 1].y == whitePieces[i]->GetSprite().getPosition().y - 96)) {
								legalMoves.pop_back();
								legalMovesIncludingFriendlyFire.pop_back();
								return;
							}
						}
						else {
							for (int j = 0; j < whitePieces[i]->GetLegalMoves().size(); j++) { //Can't move into enemy space
								if (legalMoves[legalMoves.size() - 1] == whitePieces[i]->GetLegalMoves()[j]) {
									legalMoves.pop_back();
									legalMovesIncludingFriendlyFire.pop_back();
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	catch (const out_of_range& e) {
		legalMoves.pop_back(); //The space wasn't on the board
		legalMovesIncludingFriendlyFire.pop_back();
		return;
		//cout << e.what() << endl; //DEBUG
	}
	
	//Scan around the proposed move for an enemy king
	try {
		bool exit = false;
		for (int rowOffsetTwo = -1; rowOffsetTwo <= 1; rowOffsetTwo++) {
			for (int colOffsetTwo = -1; colOffsetTwo <= 1; colOffsetTwo++) {
				ChessPiece* target = gridPieces.at(activeRowIndex + rowOffset + rowOffsetTwo).at(activeColIndex + colOffset + colOffsetTwo);
				if (target != nullptr && target->GetColor() != this->color && target->GetName() == "King") {
					legalMoves.pop_back();
					legalMovesIncludingFriendlyFire.pop_back();
					return;
				}
			}
		}
	}
	catch (const out_of_range& e) {
		//cout << e.what() << endl;
	}
}

void King::CheckCanCastle(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	//2. The left rook has not moved
	//3. The king is not in check
	//4. The spaces in between are not occupied
	ChessPiece* leftRook = gridPieces[activeRowIndex][activeColIndex - 4]; //Knowing the king hasn't moved, this is where the left rook should be
	if (!inCheck && leftRook != nullptr && leftRook->GetMoveCount() == 0 && gridPieces[activeRowIndex][activeColIndex - 1] == nullptr && gridPieces[activeRowIndex][activeColIndex - 2] == nullptr && gridPieces[activeRowIndex][activeColIndex - 3] == nullptr) {
		//5. None of the spaces in between are being attacked
		//6. The target of the king's move is not being attacked
		if (color) { //White king is attempting to castle left, see if black is attacking any of the involved tiles
			for (unsigned int i = 0; i < blackPieces.size(); i++) {
				for (unsigned int j = 0; j < blackPieces[i]->GetLegalMoves().size(); j++) {
					if (blackPieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX - (96 * 1), activePieceOriginalPositionY) ||
						blackPieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX - (96 * 2), activePieceOriginalPositionY)) {
						//The space in between or the target square is being attacked. White king cannot castle left.
						canCastleLeft = false;
					}
				}
			}
			//None of the relevant spaces were attacked, white king may castle left
			if (canCastleLeft) {
				AddKingMove(gridPieces, activeRowIndex, 0, activeColIndex, -2, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
			}
		}
		else { //Black king is attempting to castle left, see if white is attacking any of the involved tiles
			for (unsigned int i = 0; i < whitePieces.size(); i++) {
				for (unsigned int j = 0; j < whitePieces[i]->GetLegalMoves().size(); j++) {
					if (whitePieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX - (96 * 1), activePieceOriginalPositionY) ||
						whitePieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX - (96 * 2), activePieceOriginalPositionY)) {
						//The space in between or the target square is being attacked. White king cannot castle left.
						canCastleLeft = false;
					}
				}
			}
			//None of the relevant spaces were attacked, white king may castle left
			if (canCastleLeft) {
				AddKingMove(gridPieces, activeRowIndex, 0, activeColIndex, -2, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
			}
		}
	}
	else {
		//The rook wasn't there or has previously moved, or the spaces in between were occupied, or the king is in check. Cannot castle left
		canCastleLeft = false;
	}

	//Now checking Castle Right
	ChessPiece* rightRook = gridPieces[activeRowIndex][activeColIndex + 3]; //Knowing the king hasn't moved, this is where the right rook should be
	if (!inCheck && rightRook != nullptr && rightRook->GetMoveCount() == 0 && gridPieces[activeRowIndex][activeColIndex + 1] == nullptr && gridPieces[activeRowIndex][activeColIndex + 2] == nullptr) {
		//5. None of the spaces in between are being attacked
		//6. The target of the king's move is not being attacked
		if (color) { //White king is attempting to castle right, see if black is attacking any of the involved tiles
			for (unsigned int i = 0; i < blackPieces.size(); i++) {
				for (unsigned int j = 0; j < blackPieces[i]->GetLegalMoves().size(); j++) {
					if (blackPieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX + (96 * 1), activePieceOriginalPositionY) ||
						blackPieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX + (96 * 2), activePieceOriginalPositionY)) {
						//The space in between or the target square is being attacked. White king cannot castle right.
						canCastleRight = false;
					}
				}
			}
			//None of the relevant spaces were attacked, white king may castle right
			if (canCastleRight) {
				AddKingMove(gridPieces, activeRowIndex, 0, activeColIndex, 2, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
			}
		}
		else { //Black king is attempting to castle right, see if white is attacking any of the involved tiles
			for (unsigned int i = 0; i < whitePieces.size(); i++) {
				for (unsigned int j = 0; j < whitePieces[i]->GetLegalMoves().size(); j++) {
					if (whitePieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX + (96 * 1), activePieceOriginalPositionY) ||
						whitePieces[i]->GetLegalMoves()[j] == sf::Vector2f(activePieceOriginalPositionX + (96 * 2), activePieceOriginalPositionY)) {
						//The space in between or the target square is being attacked. White king cannot castle right.
						canCastleRight = false;
					}
				}
			}
			//None of the relevant spaces were attacked, white king may castle right
			if (canCastleRight) {
				AddKingMove(gridPieces, activeRowIndex, 0, activeColIndex, 2, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
			}
		}
	}
	else {
		//The rook wasn't there or has previously moved, or the spaces in between were occupied, or the king is in check. Cannot castle right
		canCastleRight = false;
	}

	if (!(canCastleRight || canCastleLeft)) {
		canCastle = false;
	}
}

void King::CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) {
	legalMoves.clear();
	legalMovesIncludingFriendlyFire.clear();
	if (moveCount == 0 && (activeRowIndex == 0 && activeColIndex == 4) || (activeRowIndex == 7 && activeColIndex == 4)) { //1. The king has not moved
		//Assume the king can castle everywhere first
		canCastle = true;
		canCastleLeft = true;
		canCastleRight = true;

		CheckCanCastle(gridPieces, activeRowIndex, activeColIndex, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	}
	else {
		canCastle = false;
		//By now, all castling conditions are thinned
	}

	AddKingMove(gridPieces, activeRowIndex, -1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, -1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, -1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, 0, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, 0, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, 1, activeColIndex, -1, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, 1, activeColIndex, 0, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);
	AddKingMove(gridPieces, activeRowIndex, 1, activeColIndex, 1, activePieceOriginalPositionX, activePieceOriginalPositionY, whitePieces, blackPieces);

	cout << "The king has " << legalMoves.size() << " moves" << endl; //DEBUG
	totalMoves = legalMoves;
}