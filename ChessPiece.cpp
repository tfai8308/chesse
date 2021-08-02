#include "ChessPiece.h"

sf::Sprite& ChessPiece::GetSprite() {
	return pieceType;
}

bool ChessPiece::GetColor() {
	return color;
}

void ChessPiece::Draw(sf::RenderWindow& window) {
	window.draw(pieceType);
}

bool ChessPiece::IsDragging() {
	return dragging;
}

void ChessPiece::SetDragging(bool drag) {
	dragging = drag;
}

void ChessPiece::IncrementMoveCount() {
	moveCount++;
}

vector<sf::Vector2f>& ChessPiece::GetLegalMoves() {
	return legalMoves;
}

vector<sf::Vector2f>& ChessPiece::GetLegalMovesWithFF() {
	return legalMovesIncludingFriendlyFire;
}

vector<sf::Vector2f>& ChessPiece::GetTotalMoves() {
	return totalMoves;
}

void ChessPiece::SetLegalMoves(const vector<sf::Vector2f>& moves) {
	legalMoves = moves;
}

void ChessPiece::AddMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY) {
	legalMoves.push_back(sf::Vector2f(activePieceOriginalPositionX + (96 * colOffset), activePieceOriginalPositionY + (96 * rowOffset)));
	legalMovesIncludingFriendlyFire.push_back(legalMoves[legalMoves.size() - 1]); //This list is more or less a copy of the other one

	try {
		ChessPiece* target = gridPieces.at(activeRowIndex + rowOffset).at(activeColIndex + colOffset);
		if (target != nullptr) { //Can't attack friendlies
			if (target->GetColor() == this->GetColor()) {
				legalMoves.pop_back();
			}
			lineOfSight = false;
			return;
		}
	}
	catch (const out_of_range& e) {
		legalMoves.pop_back(); //The space wasn't on the board
		legalMovesIncludingFriendlyFire.pop_back();
		lineOfSight = false;
		return;
		//cout << e.what() << endl; //DEBUG
	}
}

void ChessPiece::AddTotalMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY) {
	totalMoves.push_back(sf::Vector2f(activePieceOriginalPositionX + (96 * colOffset), activePieceOriginalPositionY + (96 * rowOffset)));
	try {
		ChessPiece* target = gridPieces.at(activeRowIndex + rowOffset).at(activeColIndex + colOffset);
	}
	catch (const out_of_range& e) {
		totalMoves.pop_back(); //The space wasn't on the board
		lineOfSight = false; //Only stop adding moves if they're not on the board
		//cout << e.what() << endl; //DEBUG
	}
}


int ChessPiece::GetMoveCount() {
	return moveCount;
}

string ChessPiece::GetName() {
	return name;
}

bool ChessPiece::CanCastle() {
	return canCastle;
}

bool ChessPiece::CanCastleRight() {
	return canCastleRight;
}

bool ChessPiece::CanCastleLeft() {
	return canCastleLeft;
}

bool ChessPiece::CanEnPassant() {
	return canEnPassant;
}

void ChessPiece::SetCheckState(bool check) {
	inCheck = check;
}

void ChessPiece::SetAttackingKing(bool attack) {
	attackingKing = attack;
}

bool ChessPiece::IsAttackingKing() {
	return attackingKing;
}

bool ChessPiece::IsInCheck() {
	return inCheck;
}

void ChessPiece::Move(sf::Vector2i& mousePos) {
	cout << "Moving Piece" << endl;
	pieceType.setPosition(mousePos.x - pieceType.getGlobalBounds().width / 2, mousePos.y - pieceType.getGlobalBounds().height / 2);
}

void ChessPiece::SetProtecting(bool protect) {
	protectingKing = protect;
}

bool ChessPiece::IsProtecting() {
	return protectingKing;
}

bool ChessPiece::IsAlreadyModified() {
	return alreadyModified;
}
void ChessPiece::SetAlreadyModified(bool mod) {
	alreadyModified = mod;
}

void ChessPiece::SetMoveCount(int moveCount) {
	this->moveCount = moveCount;
}

//DEBUG
void ChessPiece::PrintName() {
	cout << name << "  ";
}

void ChessPiece::PrintMoveCount() {
	cout << "This piece has made " << moveCount << " moves" << endl;
}

void ChessPiece::PrintCheckState() {
	if (color && name == "King") {
		cout << "The White king is in check: " << inCheck << endl;
	}
	if (!color && name == "King") {
		cout << "The Black king is in check: " << inCheck << endl;
	}
}

void ChessPiece::DisplayPosition() {
	cout << "Sprite at (" << pieceType.getPosition().x << ", " << pieceType.getPosition().y << ") clicked!" << endl;
}