#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "TextureManager.h"
#include "Log.h"

using namespace std;

class ChessPiece {
protected:
	sf::Sprite pieceType;
	//White = true, Black = false;
	bool color;
	bool dragging;
	int moveCount;
	bool lineOfSight; //Determines if a piece is blocked
	bool canEnPassant;
	bool canCastle;
	bool canCastleLeft;
	bool canCastleRight;
	bool inCheck;
	bool attackingKing;
	bool protectingKing;
	bool alreadyModified;

	vector<sf::Vector2f> legalMoves;
	vector<sf::Vector2f> legalMovesIncludingFriendlyFire; //A copy of legal moves, only used to determine if the King can capture something that's protected or not.
	vector<sf::Vector2f> totalMoves; //Only used if attacking King, serves as a monitor list that ignores LoS checks, only useful for Rooks, Bishops, and Queens
									 //Other pieces will observe that legalMoves == totalMoves

	string name;

public:
	sf::Sprite& GetSprite();
	bool GetColor();
	void DisplayPosition();
	void Draw(sf::RenderWindow& window);
	virtual void CalculateLegalMoves(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int activeColIndex, int activePieceOriginalPositionX, int activePieceOriginalPositionY, vector<Log>& logs, vector<ChessPiece*>& whitePieces, vector<ChessPiece*>& blackPieces) = 0;
	void AddMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY);
	void AddTotalMove(vector<vector<ChessPiece*>>& gridPieces, int activeRowIndex, int rowOffset, int activeColIndex, int colOffset, int activePieceOriginalPositionX, int activePieceOriginalPositionY);
	vector<sf::Vector2f>& GetLegalMoves();
	vector<sf::Vector2f>& GetLegalMovesWithFF();
	vector<sf::Vector2f>& GetTotalMoves();
	void SetLegalMoves(const vector<sf::Vector2f>& moves);
	void Move(sf::Vector2i& mousePos);
	bool IsDragging();
	void SetDragging(bool drag);
	void IncrementMoveCount();
	int GetMoveCount();
	string GetName();
	bool CanEnPassant();
	bool CanCastle();
	bool CanCastleRight();
	bool CanCastleLeft();
	void SetCheckState(bool check);
	void SetAttackingKing(bool attack);
	bool IsAttackingKing();
	bool IsInCheck();
	void SetProtecting(bool protect);
	bool IsProtecting();
	bool IsAlreadyModified();
	void SetAlreadyModified(bool mod);
	void SetMoveCount(int moveCount);

	void PrintName(); //DEBUG
	void PrintMoveCount(); //DEBUG
	void PrintCheckState(); //DEBUG
};