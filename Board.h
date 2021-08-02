#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Dummy.h"
#include "Log.h"
#include "AlgebraicLog.h"
#include "Dot.h"
#include "EndBanner.h"

using namespace std;

class Board {
private:
	//An 8x8 standard chessboard
	static vector<vector<Tile>> gridTiles;
	static vector<vector<ChessPiece*>> gridPieces;
	static vector<Dot> gridDots;
	//static vector<vector<sf::Vector2f>*> whiteLegalMoves;
	//static vector<vector<sf::Vector2f>*> blackLegalMoves;
	static vector<ChessPiece*> whitePieces;
	static vector<ChessPiece*> blackPieces;
	static vector<Log> logs;
	static vector<AlgebraicLog> algebraicLogs;
	static ChessPiece* activePiece;
	static ChessPiece* whiteKing;
	static ChessPiece* blackKing;
	static int activePieceOriginalPositionX;
	static int activePieceOriginalPositionY;
	static int originalRowIndex;
	static int originalColIndex;
	static bool isDragging;

	static bool turn;
	static int moveCounter;
	static bool whiteInCheck;
	static bool blackInCheck;

	static int gameState;

	static void LoadSpaces();
	static void LoadPieces();
	static bool IsLegalMove();
	static void TrackAllPieces();
	static void TrackKingMoves();
	//static void UpdateAllLegalMoves();
	//static void ClearMoveTracking();
	static void CheckKingInCheck();
	static void ReduceMovesInCheck();
	static void ReduceMovesInDoubleCheck();
	static void FindSelfRevealedChecks();
	static void ReduceSelfRevealedChecks();
	static void ResetModificationState();
	static void ExecuteMove(sf::RenderWindow& window);
	static void CheckDraw(sf::RenderWindow& window);
	static void CheckMoveCounterReset(int row, int col);
	static void CheckCheckmate(sf::RenderWindow& window);
	static int ConvertPxToIndx(float px);

	static void AddLog(string name, int startRow, int startCol, int endRow, int endCol);
	static vector<Log>& GetLogs();
	static void AddAlgebraicLog(string name, int startRow, int startCol, int endRow, int endCol, bool isCapture, bool isPromotion, int promotionType, bool isCastle, int castleType, bool isEnPassant);
	static vector<AlgebraicLog>& GetAlgebraicLogs();

	static void DrawBoard(sf::RenderWindow& window);
	static void DrawPieces(sf::RenderWindow& window);
	static void SnapToTile(sf::Vector2i& mousePos);
	static void UpdatePieceGrid(sf::RenderWindow& window);

	static void GetMovementOptions();
	static void DisplayMovementOptions(sf::RenderWindow& window);

public:
	static const unsigned int ROW_COUNT;
	static const unsigned int COL_COUNT;
	static const unsigned int TILE_SIZE;
	static bool pieceClicked;

	enum GameState { ACTIVE = 0, DRAW = 1, CHECKMATE = 2 };

	static void InitializeBoard();
	static int GetGameState();
	static ChessPiece* GetActivePiece();
	static void ClickEvent(sf::RenderWindow& window, sf::Vector2i& mousePos, bool& pieceClicked);
	static void DragEvent(sf::RenderWindow& window, sf::Vector2i& mousePos);
	static void ReleasePiece(sf::RenderWindow& window, sf::Vector2i& mousePos);
	static void ResetPiecePosition(sf::RenderWindow& window);
	static void UpdateWindow(sf::RenderWindow& window);
	static void OverlayDots(sf::RenderWindow& window);
	static void ResetDots();
	~Board();

	//DEBUG
	static void DisplayPieceGrid();
	static void DisplayMovesLists();
	static void DisplayKingCheck();
	static void DisplayAttackingPieces();
	static void ShowDotListSize();
	static void PrintMoveCounter();
	static void PrintLogs();
	static void PrintAlgebraicLogs();
	static void PrintProtectingPieces();
};