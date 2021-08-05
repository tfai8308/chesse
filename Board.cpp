#include "Board.h"

vector<vector<Tile>> Board::gridTiles;
vector<vector<ChessPiece*>> Board::gridPieces;
vector<Dot> Board::gridDots;
//vector<vector<sf::Vector2f>*> Board::whiteLegalMoves;
//vector<vector<sf::Vector2f>*> Board::blackLegalMoves;
vector<ChessPiece*> Board::whitePieces;
vector<ChessPiece*> Board::blackPieces;
vector<Log> Board::logs;
vector<AlgebraicLog> Board::algebraicLogs;
ChessPiece* Board::activePiece = nullptr;
ChessPiece* Board::whiteKing = nullptr;
ChessPiece* Board::blackKing = nullptr;
int Board::activePieceOriginalPositionX;
int Board::activePieceOriginalPositionY;
int Board::originalRowIndex;
int Board::originalColIndex;
bool Board::pieceClicked = false;
bool Board::isDragging = false;
bool Board::turn = true; //White goes first
bool Board::whiteInCheck = false;
bool Board::blackInCheck = false;
int Board::moveCounter = 0;
int Board::gameState;
const unsigned int Board::ROW_COUNT = 8;
const unsigned int Board::COL_COUNT = 8;
const unsigned int Board::TILE_SIZE = 96;

void Board::InitializeBoard() {
	//Resizing containers
	gridTiles.resize(ROW_COUNT);
	gridPieces.resize(ROW_COUNT);
	for (unsigned int i = 0; i < gridTiles.size(); i++) {
		gridTiles[i].resize(COL_COUNT);
		gridPieces[i].resize(COL_COUNT);
	}

	//Ensure all pointers are null
	for (unsigned int i = 0; i < gridPieces.size(); i++) {
		for (unsigned int j = 0; j < gridPieces[i].size(); j++) {
			gridPieces[i][j] = nullptr;
		}
	}

	LoadSpaces();
	LoadPieces();

	//Track all the legal moves of all pieces
	TrackAllPieces();

	////USED FOR ALTERNATIVE STARTING POSITIONS
	//CheckKingInCheck();
	//if (whiteInCheck || blackInCheck) {
	//	ReduceMovesInCheck();
	//	ReduceMovesInDoubleCheck();
	//}
	//else {
	//	FindSelfRevealedChecks(); //A piece can't put its own king in check by moving
	//	ReduceSelfRevealedChecks();
	//}
	//TrackKingMoves();

	gameState = GameState::ACTIVE;
}

void Board::LoadSpaces() {
	//Give each tile an X and Y position relative to the application window
	//Where 96x96 are the tile dimensions 
	int yPos = 0;
	int xPos = 0;
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		yPos = row * TILE_SIZE;
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			xPos = col * TILE_SIZE;
			gridTiles[row][col].GetSprite().setPosition(xPos, yPos);
		}
	}

	//Apply each tile's appropriate texture
	bool lightColor = true;
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			if (lightColor) {
				gridTiles[row][col].SetTexture("light_tile");
			}
			else {
				gridTiles[row][col].SetTexture("dark_tile");
			}
			lightColor = !lightColor;
		}
		lightColor = !lightColor; //Alternate colors across rows as well
	}
}

void Board::LoadPieces() {
	//Loading pieces according to a standard 8x8 chessboard, white on bottom, black on top
	//Loading Black Pawns on row index 1, columns all
	for (unsigned int col = 0; col < COL_COUNT; col++) {
		//Create Pawn
		gridPieces[1][col] = new Pawn(false);
		//Relocate sprite
		gridPieces[1][col]->GetSprite().setPosition(gridTiles[1][col].GetPosition());
	}
	
	//Loading White Pawns on row index 6, columns all
	for (unsigned int col = 0; col < COL_COUNT; col++) {
		gridPieces[6][col] = new Pawn(true);
		gridPieces[6][col]->GetSprite().setPosition(gridTiles[6][col].GetPosition());
	}
	
	//Loading Black Knights on row index 0, columns index 1, 6
	gridPieces[0][1] = new Knight(false);
	gridPieces[0][6] = new Knight(false);
	gridPieces[0][1]->GetSprite().setPosition(gridTiles[0][1].GetPosition());
	gridPieces[0][6]->GetSprite().setPosition(gridTiles[0][6].GetPosition());
	
	//Loading White Knights on row index 7, columns index 1, 6
	gridPieces[7][1] = new Knight(true);
	gridPieces[7][6] = new Knight(true);
	gridPieces[7][1]->GetSprite().setPosition(gridTiles[7][1].GetPosition());
	gridPieces[7][6]->GetSprite().setPosition(gridTiles[7][6].GetPosition());
	
	//Loading Black Bishops on row index 0, columns index 2, 5
	gridPieces[0][2] = new Bishop(false);
	gridPieces[0][5] = new Bishop(false);
	gridPieces[0][2]->GetSprite().setPosition(gridTiles[0][2].GetPosition());
	gridPieces[0][5]->GetSprite().setPosition(gridTiles[0][5].GetPosition());

	//Loading White Bishops on row index 7, columns index 2, 5
	gridPieces[7][2] = new Bishop(true);
	gridPieces[7][5] = new Bishop(true);
	gridPieces[7][2]->GetSprite().setPosition(gridTiles[7][2].GetPosition());
	gridPieces[7][5]->GetSprite().setPosition(gridTiles[7][5].GetPosition());
	
	//Loading Black Rooks on row index 0, columns index 0, 7
	gridPieces[0][0] = new Rook(false);
	gridPieces[0][7] = new Rook(false);
	gridPieces[0][0]->GetSprite().setPosition(gridTiles[0][0].GetPosition());
	gridPieces[0][7]->GetSprite().setPosition(gridTiles[0][7].GetPosition());
	
	//Loading White Rooks on row index 7, columns index 0, 7
	gridPieces[7][0] = new Rook(true);
	gridPieces[7][7] = new Rook(true);
	gridPieces[7][0]->GetSprite().setPosition(gridTiles[7][0].GetPosition());
	gridPieces[7][7]->GetSprite().setPosition(gridTiles[7][7].GetPosition());
	
	//Loading Black Queen on row index 0, column index 3
	gridPieces[0][3] = new Queen(false);
	gridPieces[0][3]->GetSprite().setPosition(gridTiles[0][3].GetPosition());
	
	//Loading White Queen on row index 7, column index 3
	gridPieces[7][3] = new Queen(true);
	gridPieces[7][3]->GetSprite().setPosition(gridTiles[7][3].GetPosition());
	
	//Loading Black King on row index 0, column index 4, track it
	gridPieces[0][4] = new King(false);
	gridPieces[0][4]->GetSprite().setPosition(gridTiles[0][4].GetPosition());
	blackKing = gridPieces[0][4];
	
	//Loading White King on row index 7, column index 4, track it
	gridPieces[7][4] = new King(true);
	gridPieces[7][4]->GetSprite().setPosition(gridTiles[7][4].GetPosition());
	whiteKing = gridPieces[7][4];

	//DEBUG PIECES CAN BE ADDED HERE
}

void Board::DrawBoard(sf::RenderWindow& window) {
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			gridTiles[row][col].Draw(window);
		}
	}
}

void Board::DrawPieces(sf::RenderWindow& window) {
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			if (gridPieces[row][col] != nullptr) {
				gridPieces[row][col]->Draw(window);
			}
		}
	}
}

void Board::TrackAllPieces() {
	whitePieces.clear();
	blackPieces.clear();
	
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			if (gridPieces[row][col] != nullptr) {

				if (!gridPieces[row][col]->IsAlreadyModified() /*|| gridPieces[row][col]->IsMarkedForRecalculation()*/) {
					gridPieces[row][col]->CalculateLegalMoves(gridPieces, row, col, gridPieces[row][col]->GetSprite().getPosition().x, gridPieces[row][col]->GetSprite().getPosition().y, logs, whitePieces, blackPieces);
					//gridPieces[row][col]->SetMarkedForRecalculation(false);
				}

				if (gridPieces[row][col]->GetColor()) {
					whitePieces.push_back(gridPieces[row][col]);
				}
				else {
					blackPieces.push_back(gridPieces[row][col]);
				}
			}
		}
	}
}

void Board::TrackKingMoves() {
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			if (gridPieces[row][col] != nullptr && gridPieces[row][col]->GetName() == "King") {
				gridPieces[row][col]->CalculateLegalMoves(gridPieces, row, col, gridPieces[row][col]->GetSprite().getPosition().x, gridPieces[row][col]->GetSprite().getPosition().y, logs, whitePieces, blackPieces);
			}
		}
	}
}

void Board::ClickEvent(sf::RenderWindow& window, sf::Vector2i& mousePos, bool& pieceClicked) {
	//Iterate backwards to drag pieces from the top of the sprite pile
	bool dotClicked = false;
	for (int row = ROW_COUNT - 1; row >= 0 && !(pieceClicked || dotClicked); row--) {
		for (int col = COL_COUNT - 1; col >= 0 && !(pieceClicked || dotClicked); col--) {
			//Setting a target for whatever was clicked, so long as no dots are currently displayed (no other piece is already selected -- Sprite priority is given to the dots.)
			if (gridPieces[row][col] != nullptr && gridPieces[row][col]->GetSprite().getGlobalBounds().contains(mousePos.x, mousePos.y) && gridDots.empty()) {
				gridPieces[row][col]->DisplayPosition(); //DEBUG

				//Getting information on the piece that was clicked
				activePieceOriginalPositionX = gridPieces[row][col]->GetSprite().getPosition().x;
				activePieceOriginalPositionY = gridPieces[row][col]->GetSprite().getPosition().y;
				originalRowIndex = row;
				originalColIndex = col;
				activePiece = gridPieces[row][col];
				pieceClicked = true;
			}
			else { //Somewhere that wasn't a dot or piece was clicked
				sf::Vector2i dotPosition;
				for (unsigned int i = 0; i < gridDots.size(); i++) {
					if (gridDots[i].GetSprite().getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						dotClicked = true;
						dotPosition.x = (int) gridDots[i].GetSprite().getPosition().x;
						dotPosition.y = (int) gridDots[i].GetSprite().getPosition().y;
						break;
					}
				}

				if (!dotClicked) { //If the click happened on a space without any dots, clear the visuals
					ResetDots();
					UpdateWindow(window);
				}
				else {
					cout << "A dot was clicked!" << endl;
					SnapToTile(dotPosition);

					ExecuteMove(window);

					//Game Termination Checks
					CheckDraw(window);
					CheckCheckmate(window);
				}
			}
		}
	}
}

void Board::DragEvent(sf::RenderWindow& window, sf::Vector2i& mousePos) {
	//for (unsigned int row = 0; row < ROW_COUNT; row++) {
	//	for (unsigned int col = 0; col < COL_COUNT; col++) {
	//		if (gridPieces[row][col] != nullptr && gridPieces[row][col]->GetSprite().getGlobalBounds().contains(mousePos.x, mousePos.y) && gridPieces[row][col]->IsDragging()) {
	//			gridPieces[row][col]->Move(mousePos);
	//		}
	//	}
	//}
	if (activePiece->IsDragging()) {
		activePiece->Move(mousePos);
		Board::UpdateWindow(window);
	}
}

bool Board::IsLegalMove() {
	for (unsigned int i = 0; i < activePiece->GetLegalMoves().size(); i++) {
		if (activePiece->GetSprite().getPosition() == activePiece->GetLegalMoves()[i]) {
			return true;
		}
	}
	return false;
}

void Board::CheckKingInCheck() {
	//Assume neither king is in check to begin
	whiteInCheck = false;
	blackInCheck = false;

	//See if the white king is in check
	for (int i = 0; i < blackPieces.size(); i++) {
		bool attacking = false;
		for (int j = 0; j < blackPieces[i]->GetLegalMoves().size(); j++) {
			if (whiteKing->GetSprite().getPosition() == blackPieces[i]->GetLegalMoves()[j]) {
				attacking = true;
				whiteInCheck = true;
			}
		}
		blackPieces[i]->SetAttackingKing(attacking);
	}
	whiteKing->SetCheckState(whiteInCheck);

	//See if the black king is in check
	for (int i = 0; i < whitePieces.size(); i++) {
		bool attacking = false;
		for (int j = 0; j < whitePieces[i]->GetLegalMoves().size(); j++) {
			if (blackKing->GetSprite().getPosition() == whitePieces[i]->GetLegalMoves()[j]) {
				attacking = true;
				blackInCheck = true;
			}
		}
		whitePieces[i]->SetAttackingKing(attacking);
	}
	blackKing->SetCheckState(blackInCheck);
}

void Board::ReduceMovesInCheck() {
	//If a move takes you out of check either by blocking LoS or moving out of danger, allow it
	//If a move leaves you in check, remove it
	int row = -1, col = -1;
	vector<sf::Vector2f> moves;
	if (whiteInCheck) {
		for (unsigned int i = 0; i < whitePieces.size(); i++) {

			moves = whitePieces[i]->GetLegalMoves();

			for (unsigned int j = 0; j < moves.size(); j++) {
				
				row = ConvertPxToIndx(moves[j].y);
				col = ConvertPxToIndx(moves[j].x);
				
				if (gridPieces[row][col] == nullptr) { //Spawning dummies
					gridPieces[row][col] = new Dummy(true);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());

					TrackAllPieces(); //Simulating a move
					CheckKingInCheck(); //Checking if that moves saves you from death
				}
				
				if (gridPieces[row][col] != nullptr && !gridPieces[row][col]->GetColor() && gridPieces[row][col]->IsAttackingKing()) {
					//Allow the move to kill the attacker
				}
				else {
					if (whiteInCheck) {
						moves.erase(moves.begin() + j);
						j--;
					}
				}

				if (gridPieces[row][col]->GetName() == "Dumm") {
					delete gridPieces[row][col]; //If a dummy was spawned at [row][col], get rid of it right away as well
					gridPieces[row][col] = nullptr;
				}
				TrackAllPieces(); //Retrack to pretend nothing ever happened
				CheckKingInCheck(); //Reset the check state as well
			}

			whitePieces[i]->SetLegalMoves(moves);
			whitePieces[i]->SetAlreadyModified(true);
		}

		/*for (unsigned int i = 0; i < movesToDelete.size(); i++) {
			for (unsigned int j = 0; j < whitePieces.size(); j++) {
				for (unsigned int k = 0; k < whitePieces[j]->GetLegalMoves().size(); k++) {
					if (movesToDelete[i] == whitePieces[j]->GetLegalMoves()[k]) {
						whitePieces[j]->GetLegalMoves().erase(whitePieces[j]->GetLegalMoves().begin() + k);
					}
				}
			}
		}*/
	}
	if (blackInCheck) {
		for (unsigned int i = 0; i < blackPieces.size(); i++) {

			moves = blackPieces[i]->GetLegalMoves();

			for (unsigned int j = 0; j < moves.size(); j++) {
				
				row = ConvertPxToIndx(moves[j].y);
				col = ConvertPxToIndx(moves[j].x);
				
				if (gridPieces[row][col] == nullptr) { //Spawning dummies
					gridPieces[row][col] = new Dummy(false);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
					
					TrackAllPieces(); //Simulating a move
					CheckKingInCheck(); //Checking if that moves saves you from death
				}
				
				if (gridPieces[row][col] != nullptr && gridPieces[row][col]->GetColor() && gridPieces[row][col]->IsAttackingKing()) {
					//Allow the move to kill the attacker
				}
				else {
					if (blackInCheck) { //If the King is still in check, prepare to delete the move
						moves.erase(moves.begin() + j);
						j--;
					}
				}
				
				if (gridPieces[row][col]->GetName() == "Dumm") {
					delete gridPieces[row][col]; //Also get rid of that dummy right away
					gridPieces[row][col] = nullptr;
				}	

				TrackAllPieces(); //Retrack to pretend nothing ever happened
				CheckKingInCheck(); //Reset the check state as well
			}

			blackPieces[i]->SetLegalMoves(moves);
			blackPieces[i]->SetAlreadyModified(true);
		}

		/*for (unsigned int i = 0; i < movesToDelete.size(); i++) {
			for (unsigned int j = 0; j < blackPieces.size(); j++) {
				for (unsigned int k = 0; k < blackPieces[j]->GetLegalMoves().size(); k++) {
					if (movesToDelete[i] == blackPieces[j]->GetLegalMoves()[k]) {
						blackPieces[j]->GetLegalMoves().erase(blackPieces[j]->GetLegalMoves().begin() + k);
					}
				}
			}
		}*/
	}

	ResetModificationState();
}

void Board::ReduceMovesInDoubleCheck() {
	//The king is being attacked from multiple places, only the king may save itself by moving
	int numAttackers = 0;
	if (whiteInCheck) {
		for (int i = 0; i < blackPieces.size(); i++) {
			if (blackPieces[i]->IsAttackingKing()) {
				numAttackers++;
			}
		}

		if (numAttackers > 1) {
			for (int i = 0; i < whitePieces.size(); i++) {
				if (whitePieces[i]->GetName() != "King") {
					whitePieces[i]->GetLegalMoves().clear();
				}
			}
		}
	}
	if (blackInCheck) {
		for (int i = 0; i < whitePieces.size(); i++) {
			if (whitePieces[i]->IsAttackingKing()) {
				numAttackers++;
			}
		}

		if (numAttackers > 1) {
			for (int i = 0; i < blackPieces.size(); i++) {
				if (blackPieces[i]->GetName() != "King") {
					blackPieces[i]->GetLegalMoves().clear();
				}
			}
		}
	}
}

void Board::FindSelfRevealedChecks() {
	//Scan the entire board, find pieces that are protecting their own king
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			ChessPiece* target = gridPieces[row][col];
			if (target != nullptr && target->GetName() != "King") {
				
				string targetType = target->GetName(); //Save some information to reconstruct the piece in place
				bool targetColor = target->GetColor();
				int moveCount = target->GetMoveCount();

				target->SetProtecting(false); //Assume it's not protecting anything to begin with
				
				delete gridPieces[row][col];
				gridPieces[row][col] = nullptr;

				TrackAllPieces();
				CheckKingInCheck();
				
				//Begin reconstructing the piece
				if (targetType == "Pawn") {
					gridPieces[row][col] = new Pawn(targetColor);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
				}
				else if (targetType == "Nnig") {
					gridPieces[row][col] = new Knight(targetColor);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
				}
				else if (targetType == "Bish") {
					gridPieces[row][col] = new Bishop(targetColor);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
				}
				else if (targetType == "Rook") {
					gridPieces[row][col] = new Rook(targetColor);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
				}
				else if (targetType == "Quee") {
					gridPieces[row][col] = new Queen(targetColor);
					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
				}

				if ((whiteInCheck && targetColor) || (blackInCheck && !targetColor)) { //Deleting the piece put its own king in check
					gridPieces[row][col]->SetProtecting(true);
				}

				TrackAllPieces(); //Undo the simulated move
				gridPieces[row][col]->SetMoveCount(moveCount); //Finish reconstructing the piece
				CheckKingInCheck();
			}
		}
	}
}

void Board::ReduceSelfRevealedChecks() {
	//Of the pieces potentially protecting the king, simulate and remove moves that put the king in check
	//int row = -1, col = -1;
	//int moveCount;
	//for (unsigned int i = 0; i < whitePieces.size(); i++) {
	//	if (whitePieces[i]->IsProtecting()) {
	//		
	//		//Save some data and temporarily remove the piece again
	//		vector<sf::Vector2f> moves = whitePieces[i]->GetLegalMoves();
	//		string targetType = whitePieces[i]->GetName(); //Save some information to reconstruct the piece in place
	//		moveCount = whitePieces[i]->GetMoveCount();

	//		int originalRow = ConvertPxToIndx(whitePieces[i]->GetSprite().getPosition().y);
	//		int originalCol = ConvertPxToIndx(whitePieces[i]->GetSprite().getPosition().x);
	//		delete gridPieces[originalRow][originalCol];
	//		gridPieces[originalRow][originalCol] = nullptr;
	//		TrackAllPieces(); //Simulating a move
	//		CheckKingInCheck();

	//		for (unsigned int j = 0; j < moves.size(); j++) {
	//			bool enemyWasDeleted = false;
	//			row = ConvertPxToIndx(moves[j].y);
	//			col = ConvertPxToIndx(moves[j].x);

	//			if (gridPieces[row][col] == nullptr) { //Spawning dummies
	//				gridPieces[row][col] = new Dummy(true);
	//				gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());

	//				TrackAllPieces(); //Simulating a move
	//				CheckKingInCheck(); //Checking if that moves saves you from death
	//			}

	//			//Saving enemy data
	//			vector<sf::Vector2f> enemyMoves;
	//			string enemyType = "";
	//			int enemyMoveCount = -1;
	//			if (gridPieces[row][col] != nullptr && !gridPieces[row][col]->GetColor()) {
	//				enemyWasDeleted = true;
	//				enemyMoves = gridPieces[row][col]->GetLegalMoves();
	//				enemyType = gridPieces[row][col]->GetName(); //Save some information to reconstruct the piece in place
	//				enemyMoveCount = gridPieces[row][col]->GetMoveCount();

	//				delete gridPieces[row][col];
	//				gridPieces[row][col] = nullptr;
	//				gridPieces[row][col] = new Dummy(true);
	//				gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());

	//				TrackAllPieces(); //Simulating a move
	//				CheckKingInCheck(); //Checking if that moves saves you from death
	//			}

	//			if (gridPieces[row][col] != nullptr && !gridPieces[row][col]->GetColor() && gridPieces[row][col]->IsAttackingKing()) {
	//				//Allow the move to kill the attacker
	//			}
	//			else {
	//				if (whiteInCheck) {
	//					moves.erase(moves.begin() + j);
	//					j--;
	//				}
	//			}

	//			if (gridPieces[row][col]->GetName() == "Dumm") {
	//				delete gridPieces[row][col]; //Also get rid of that dummy right away
	//				gridPieces[row][col] = nullptr;
	//			}

	//			//Reconstruct the enemy
	//			if (enemyWasDeleted) {
	//				if (enemyType == "Pawn") {
	//					gridPieces[row][col] = new Pawn(false);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Nnig") {
	//					gridPieces[row][col] = new Knight(false);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Bish") {
	//					gridPieces[row][col] = new Bishop(false);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Rook") {
	//					gridPieces[row][col] = new Rook(false);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Quee") {
	//					gridPieces[row][col] = new Queen(false);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				gridPieces[row][col]->SetLegalMoves(enemyMoves);
	//				gridPieces[row][col]->SetMoveCount(enemyMoveCount);
	//				TrackAllPieces();
	//			}
	//		}

	//		//Begin reconstructing the piece
	//		if (targetType == "Pawn") {
	//			gridPieces[originalRow][originalCol] = new Pawn(true);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Nnig") {
	//			gridPieces[originalRow][originalCol] = new Knight(true);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Bish") {
	//			gridPieces[originalRow][originalCol] = new Bishop(true);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Rook") {
	//			gridPieces[originalRow][originalCol] = new Rook(true);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Quee") {
	//			gridPieces[originalRow][originalCol] = new Queen(true);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}


	//		TrackAllPieces(); //Undoing a simulated move
	//		gridPieces[originalRow][originalCol]->SetLegalMoves(moves); //Redefining that piece's moveset
	//		gridPieces[originalRow][originalCol]->SetAlreadyModified(true); //Do not allow retracking to cancel the change to the moveset
	//		gridPieces[originalRow][originalCol]->SetMoveCount(moveCount); //Finish reconstructing the piece
	//		CheckKingInCheck();
	//	}
	//}

	//for (unsigned int i = 0; i < blackPieces.size(); i++) {
	//	if (blackPieces[i]->IsProtecting()) {

	//		//Save some data and temporarily remove the piece again
	//		vector<sf::Vector2f> moves = blackPieces[i]->GetLegalMoves();
	//		string targetType = blackPieces[i]->GetName(); //Save some information to reconstruct the piece in place
	//		moveCount = blackPieces[i]->GetMoveCount();

	//		int originalRow = ConvertPxToIndx(blackPieces[i]->GetSprite().getPosition().y);
	//		int originalCol = ConvertPxToIndx(blackPieces[i]->GetSprite().getPosition().x);
	//		delete gridPieces[originalRow][originalCol];
	//		gridPieces[originalRow][originalCol] = nullptr;
	//		TrackAllPieces(); //Simulating a move
	//		CheckKingInCheck();

	//		for (unsigned int j = 0; j < moves.size(); j++) {
	//			bool enemyWasDeleted = false;
	//			row = ConvertPxToIndx(moves[j].y);
	//			col = ConvertPxToIndx(moves[j].x);

	//			if (gridPieces[row][col] == nullptr) { //Spawning dummies
	//				gridPieces[row][col] = new Dummy(false);
	//				gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());

	//				TrackAllPieces(); //Simulating a move
	//				CheckKingInCheck(); //Checking if that moves saves you from death
	//			}

	//			//Saving enemy data
	//			vector<sf::Vector2f> enemyMoves;
	//			string enemyType = "";
	//			int enemyMoveCount = -1;
	//			if (gridPieces[row][col] != nullptr && gridPieces[row][col]->GetColor()) {
	//				enemyWasDeleted = true;
	//				enemyMoves = gridPieces[row][col]->GetLegalMoves();
	//				enemyType = gridPieces[row][col]->GetName(); //Save some information to reconstruct the piece in place
	//				enemyMoveCount = gridPieces[row][col]->GetMoveCount();

	//				delete gridPieces[row][col];
	//				gridPieces[row][col] = nullptr;
	//				gridPieces[row][col] = new Dummy(false);
	//				gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());

	//				TrackAllPieces(); //Simulating a move
	//				CheckKingInCheck(); //Checking if that moves saves you from death
	//			}

	//			if (gridPieces[row][col] != nullptr && gridPieces[row][col]->GetColor() && gridPieces[row][col]->IsAttackingKing()) {
	//				//Allow the move to kill the attacker
	//			}
	//			else {
	//				if (blackInCheck) {
	//					moves.erase(moves.begin() + j);
	//					j--;
	//				}
	//			}

	//			if (gridPieces[row][col]->GetName() == "Dumm") {
	//				delete gridPieces[row][col]; //Also get rid of that dummy right away
	//				gridPieces[row][col] = nullptr;
	//			}

	//			//Reconstruct the enemy
	//			if (enemyWasDeleted) {
	//				if (enemyType == "Pawn") {
	//					gridPieces[row][col] = new Pawn(true);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Nnig") {
	//					gridPieces[row][col] = new Knight(true);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Bish") {
	//					gridPieces[row][col] = new Bishop(true);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Rook") {
	//					gridPieces[row][col] = new Rook(true);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				else if (enemyType == "Quee") {
	//					gridPieces[row][col] = new Queen(true);
	//					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
	//				}
	//				gridPieces[row][col]->SetLegalMoves(enemyMoves);
	//				gridPieces[row][col]->SetMoveCount(enemyMoveCount);
	//				TrackAllPieces();
	//			}
	//		}

	//		//Begin reconstructing the piece
	//		if (targetType == "Pawn") {
	//			gridPieces[originalRow][originalCol] = new Pawn(false);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Nnig") {
	//			gridPieces[originalRow][originalCol] = new Knight(false);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Bish") {
	//			gridPieces[originalRow][originalCol] = new Bishop(false);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Rook") {
	//			gridPieces[originalRow][originalCol] = new Rook(false);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}
	//		else if (targetType == "Quee") {
	//			gridPieces[originalRow][originalCol] = new Queen(false);
	//			gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
	//		}


	//		TrackAllPieces(); //Undoing a simulated move
	//		gridPieces[originalRow][originalCol]->SetLegalMoves(moves); //Redefining that piece's moveset
	//		gridPieces[originalRow][originalCol]->SetAlreadyModified(true); //Do not allow retracking to cancel the change to the moveset
	//		gridPieces[originalRow][originalCol]->SetMoveCount(moveCount); //Finish reconstructing the piece
	//		CheckKingInCheck();
	//	}
	//}

	//ResetModificationState();

	int simMoveRow = -1, simMoveCol = -1;
	int moveCount;
	for (int row = 0; row < ROW_COUNT; row++) {
		for (int col = 0; col < COL_COUNT; col++) {
			ChessPiece* target = gridPieces[row][col];
			if (target != nullptr && target->IsProtecting()) {

				bool protectorColor = target->GetColor(); //Figure out the protector's color T = White, F = Black

				//Save some data and temporarily remove the piece again
				vector<sf::Vector2f> moves = target->GetLegalMoves();
				string targetType = target->GetName(); //Save some information to reconstruct the piece in place
				moveCount = target->GetMoveCount();

				int originalRow = ConvertPxToIndx(target->GetSprite().getPosition().y);
				int originalCol = ConvertPxToIndx(target->GetSprite().getPosition().x);
				delete gridPieces[originalRow][originalCol];
				gridPieces[originalRow][originalCol] = nullptr;
				TrackAllPieces(); //Simulating a move
				CheckKingInCheck();

				for (unsigned int i = 0; i < moves.size(); i++) {
					bool enemyWasDeleted = false;
					simMoveRow = ConvertPxToIndx(moves[i].y);
					simMoveCol = ConvertPxToIndx(moves[i].x);

					if (gridPieces[simMoveRow][simMoveCol] == nullptr) { //Spawning dummies
						gridPieces[simMoveRow][simMoveCol] = new Dummy(protectorColor);
						gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());

						TrackAllPieces(); //Simulating a move
						CheckKingInCheck(); //Checking if that moves saves you from death
					}

					//Saving enemy data
					vector<sf::Vector2f> enemyMoves;
					string enemyType = "";
					int enemyMoveCount = -1;
					if (gridPieces[simMoveRow][simMoveCol] != nullptr && (gridPieces[simMoveRow][simMoveCol]->GetColor() != protectorColor)) {
						enemyWasDeleted = true;
						enemyMoves = gridPieces[simMoveRow][simMoveCol]->GetLegalMoves();
						enemyType = gridPieces[simMoveRow][simMoveCol]->GetName(); //Save some information to reconstruct the piece in place
						enemyMoveCount = gridPieces[simMoveRow][simMoveCol]->GetMoveCount();

						delete gridPieces[simMoveRow][simMoveCol];
						gridPieces[simMoveRow][simMoveCol] = nullptr;
						gridPieces[simMoveRow][simMoveCol] = new Dummy(protectorColor);
						gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());

						TrackAllPieces(); //Simulating a move
						CheckKingInCheck(); //Checking if that moves saves you from death
					}

					if (gridPieces[simMoveRow][simMoveCol] != nullptr && (gridPieces[simMoveRow][simMoveCol]->GetColor() != protectorColor) && gridPieces[simMoveRow][simMoveCol]->IsAttackingKing()) {
						//Allow the move to kill the attacker
					}
					else {
						if (!protectorColor && blackInCheck) {
							moves.erase(moves.begin() + i);
							i--;
						}
						else if (protectorColor && whiteInCheck) {
							moves.erase(moves.begin() + i);
							i--;
						}
					}

					if (gridPieces[simMoveRow][simMoveCol]->GetName() == "Dumm") {
						delete gridPieces[simMoveRow][simMoveCol]; //Also get rid of that dummy right away
						gridPieces[simMoveRow][simMoveCol] = nullptr;
					}

					//Reconstruct the enemy
					if (enemyWasDeleted) {
						if (enemyType == "Pawn") {
							gridPieces[simMoveRow][simMoveCol] = new Pawn(!protectorColor);
							gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());
						}
						else if (enemyType == "Nnig") {
							gridPieces[simMoveRow][simMoveCol] = new Knight(!protectorColor);
							gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());
						}
						else if (enemyType == "Bish") {
							gridPieces[simMoveRow][simMoveCol] = new Bishop(!protectorColor);
							gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());
						}
						else if (enemyType == "Rook") {
							gridPieces[simMoveRow][simMoveCol] = new Rook(!protectorColor);
							gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());
						}
						else if (enemyType == "Quee") {
							gridPieces[simMoveRow][simMoveCol] = new Queen(!protectorColor);
							gridPieces[simMoveRow][simMoveCol]->GetSprite().setPosition(gridTiles[simMoveRow][simMoveCol].GetPosition());
						}
						gridPieces[simMoveRow][simMoveCol]->SetLegalMoves(enemyMoves);
						gridPieces[simMoveRow][simMoveCol]->SetMoveCount(enemyMoveCount);
						TrackAllPieces();
					}
				}

				//Begin reconstructing the piece
				if (targetType == "Pawn") {
					gridPieces[originalRow][originalCol] = new Pawn(protectorColor);
					gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
				}
				else if (targetType == "Nnig") {
					gridPieces[originalRow][originalCol] = new Knight(protectorColor);
					gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
				}
				else if (targetType == "Bish") {
					gridPieces[originalRow][originalCol] = new Bishop(protectorColor);
					gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
				}
				else if (targetType == "Rook") {
					gridPieces[originalRow][originalCol] = new Rook(protectorColor);
					gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
				}
				else if (targetType == "Quee") {
					gridPieces[originalRow][originalCol] = new Queen(protectorColor);
					gridPieces[originalRow][originalCol]->GetSprite().setPosition(gridTiles[originalRow][originalCol].GetPosition());
				}


				TrackAllPieces(); //Undoing a simulated move
				gridPieces[originalRow][originalCol]->SetLegalMoves(moves); //Redefining that piece's moveset
				gridPieces[originalRow][originalCol]->SetAlreadyModified(true); //Do not allow retracking to cancel the change to the moveset
				gridPieces[originalRow][originalCol]->SetMoveCount(moveCount); //Finish reconstructing the piece
				CheckKingInCheck();
			}
		}
	}

	ResetModificationState();
}

void Board::ResetModificationState() {
	for (int i = 0; i < whitePieces.size(); i++) {
		if (whitePieces[i]->IsAlreadyModified()) {
			whitePieces[i]->SetAlreadyModified(false);
		}
	}

	for (int i = 0; i < blackPieces.size(); i++) {
		if (blackPieces[i]->IsAlreadyModified()) {
			blackPieces[i]->SetAlreadyModified(false);
		}
	}
}

void Board::CheckMoveCounterReset(int row, int col) {
	if (gridPieces[row][col] != nullptr || activePiece->GetName() == "Pawn") {
		moveCounter = 0;
	}
}

void Board::CheckDraw(sf::RenderWindow& window) {
	//1. Someone's out of legal moves and not in check and it's their turn
	vector<vector<sf::Vector2f>> moves;
	if (!whiteInCheck) {
		for (unsigned int i = 0; i < whitePieces.size(); i++) {
			if (!whitePieces[i]->GetLegalMoves().empty()) {
				moves.push_back(whitePieces[i]->GetLegalMoves());
			}
		}
		if (moves.empty()) {
			cout << "White is stalemated" << endl;
			EndBanner::DrawBanner(0, window);
			gameState = GameState::DRAW;
			algebraicLogs[algebraicLogs.size() - 1].AppendDrawSymbol();
			PrintAlgebraicLogs(); //Review the game
			return;
		}
	}

	moves.clear();
	if (!blackInCheck) {
		for (unsigned int i = 0; i < blackPieces.size(); i++) {
			if (!blackPieces[i]->GetLegalMoves().empty()) {
				moves.push_back(blackPieces[i]->GetLegalMoves());
			}
		}
		cout << moves.size() << endl;
		if (moves.empty()) {
			cout << "Black is stalemated" << endl;
			EndBanner::DrawBanner(0, window);
			gameState = GameState::DRAW;
			algebraicLogs[algebraicLogs.size() - 1].AppendDrawSymbol();
			PrintAlgebraicLogs(); //Review the game
			return;
		}
	}

	//2. The last 20 moves have resulted in the same position 5 times
	if (logs.size() % 4 == 0 && logs.size() >= 20) {
		int i = logs.size() - 20;
		for (; i < logs.size() - 4; i += 4) {
			cout << i << endl;
			if (!(logs[i] == logs[i + 4] && logs[i + 1] == logs[i + 5] && logs[i + 2] == logs[i + 6] && logs[i + 3] == logs[i + 7])) {
				break;
			}
		}
		if (i == logs.size() - 4) {
			cout << "The last 5 sets of moves were identical. Forced Draw." << endl;
			EndBanner::DrawBanner(0, window);
			gameState = GameState::DRAW;
			algebraicLogs[algebraicLogs.size() - 1].AppendDrawSymbol();
			PrintAlgebraicLogs(); //Review the game
		}
	}

	//3. The last 100 moves did not result in a capture or pawn move
	if (moveCounter == 100) {
		cout << "100 moves have passed since a piece was capture or a pawn moved. Game Drawn." << endl;
		EndBanner::DrawBanner(0, window);
		gameState = GameState::DRAW;
		algebraicLogs[algebraicLogs.size() - 1].AppendDrawSymbol();
		PrintAlgebraicLogs(); //Review the game
	}
}

void Board::CheckCheckmate(sf::RenderWindow& window) {
	//1. Someone's in check and out of legal moves
	vector<vector<sf::Vector2f>> moves;
	if (whiteInCheck) {
		for (unsigned int i = 0; i < whitePieces.size(); i++) {
			if (!whitePieces[i]->GetLegalMoves().empty()) {
				moves.push_back(whitePieces[i]->GetLegalMoves());
			}
		}
		if (moves.empty()) {
			cout << "White Checkmated, Black Wins" << endl;
			EndBanner::DrawBanner(2, window);
			gameState = GameState::CHECKMATE;
			algebraicLogs[algebraicLogs.size() - 1].AppendVictorySymbol(false);
			PrintAlgebraicLogs(); //Review the game
		}
	}

	if (blackInCheck) {
		for (unsigned int i = 0; i < blackPieces.size(); i++) {
			if (!blackPieces[i]->GetLegalMoves().empty()) {
				moves.push_back(blackPieces[i]->GetLegalMoves());
			}
		}
		if (moves.empty()) {
			cout << "Black Checkmated, White Wins" << endl;
			EndBanner::DrawBanner(1, window);
			gameState = GameState::CHECKMATE;
			algebraicLogs[algebraicLogs.size() - 1].AppendVictorySymbol(true);
			PrintAlgebraicLogs(); //Review the game
		}
	}
}

void Board::ReleasePiece(sf::RenderWindow& window, sf::Vector2i& mousePos) {
	//for (unsigned int row = 0; row < ROW_COUNT; row++) {
	//	for (unsigned int col = 0; col < COL_COUNT; col++) {
	//		if (gridPieces[row][col] != nullptr && gridPieces[row][col]->IsDragging()) {
	//			gridPieces[row][col]->SetDragging(false);
	//			SnapToTile(gridPieces[row][col], window, mousePos);
	//		}
	//	}
	//}
	activePiece->PrintMoveCount(); //DEBUG
	if (activePiece->IsDragging()) { //This block executes if the mouse dragged at all
		activePiece->SetDragging(false);
		SnapToTile(mousePos);

		ExecuteMove(window);
	}
	else { //If the piece was simply clicked without dragging, this block executes instead to show the possible moves

		/*>>Dots are displayed upon releasing the mouse without moving.
		  >>A move is confirmed upon the immediate click on a dot
		  >>Dots are cleared upon first immediate click outside a legal space*/

		if (activePiece->GetColor() == turn) {
			cout << "Clicked without dragging" << endl;
			UpdateWindow(window); //Start by updating the window to display the dots
			OverlayDots(window);
		}
		else {
			string color = activePiece->GetColor() ? "white's" : "black's";
			cout << "It's not " << color << " turn. Nothing displayed." << endl;
		}
	}
	
	pieceClicked = false; //Left mouse up, piece is released

	//Game Termination Checks
	CheckDraw(window);
	CheckCheckmate(window);
}

void Board::ExecuteMove(sf::RenderWindow& window) {
	if (IsLegalMove() && activePiece->GetColor() == turn) {
		cout << "Legal" << endl;
		moveCounter++;
		UpdatePieceGrid(window); //The only place a piece has potentially moved
		UpdateWindow(window); //Update the graphics before running all those calculations

		ResetDots();

		activePiece->IncrementMoveCount();

		TrackAllPieces(); //Initial piece tracking updates
		CheckKingInCheck(); //See if either king is in check

		if (whiteInCheck || blackInCheck) {
			ReduceMovesInCheck();
			ReduceMovesInDoubleCheck();
			algebraicLogs[algebraicLogs.size() - 1].AppendCheckSymbol();
		}
		else {
			FindSelfRevealedChecks(); //A piece can't put its own king in check by moving
			PrintProtectingPieces(); //DEBUG
			ReduceSelfRevealedChecks();
		}

		TrackKingMoves(); //Retrack King's moves

		turn = !turn; //Switch turns
	}
	else {
		cout << "Illegal" << endl;
		string activePieceColor = activePiece->GetColor() ? "white" : "black";
		if (activePiece->GetColor() != turn) {
			cout << "It's not " << activePieceColor << "'s turn" << endl;
		}
		ResetPiecePosition(window); //An invalid move was made
	}
}

void Board::GetMovementOptions() {
	ResetDots();
	for (unsigned int i = 0; i < activePiece->GetLegalMoves().size(); i++) {
		gridDots.push_back(Dot());
		gridDots[i].GetSprite().setPosition(activePiece->GetLegalMoves()[i].x, activePiece->GetLegalMoves()[i].y);
	}
}

void Board::DisplayMovementOptions(sf::RenderWindow& window) {
	for (unsigned int i = 0; i < gridDots.size(); i++) {
		gridDots[i].Draw(window);
	}
}

void Board::ResetDots() {
	gridDots.clear();
}

void Board::ResetPiecePosition(sf::RenderWindow& window) {
	//for (unsigned int row = 0; row < ROW_COUNT; row++) {
	//	for (unsigned int col = 0; col < COL_COUNT; col++) {
	//		if (gridPieces[row][col] != nullptr && gridPieces[row][col]->IsDragging()) {
	//			gridPieces[row][col]->GetSprite().setPosition(activePieceOriginalPositionX, activePieceOriginalPositionY);
	//			gridPieces[row][col]->SetDragging(false);
	//		}
	//	}
	//}
	//Reasons to reset:
	//1. The move wasn't legal
	//2. The move is off the screen
	//3. It's not that piece color's turn
	if (!IsLegalMove() || activePiece->IsDragging() || activePiece->GetColor() != turn) {
		cout << "Reset Position" << endl;
		activePiece->GetSprite().setPosition(activePieceOriginalPositionX, activePieceOriginalPositionY);
		activePiece->SetDragging(false);
		UpdateWindow(window);
	}
	pieceClicked = false; //Out of bounds, piece is released
}

void Board::SnapToTile(sf::Vector2i& mousePos) {
	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			if (gridTiles[row][col].GetSprite().getGlobalBounds().contains(mousePos.x, mousePos.y)) {
				activePiece->GetSprite().setPosition(gridTiles[row][col].GetPosition());
			}
		}
	}
}

void Board::UpdatePieceGrid(sf::RenderWindow& window) {

	//Logger Information
	bool isCapture = false;
	bool isPromotion = false;
	bool isCastle = false;
	bool isEnPassant = false;
	int promotionType = -1;
	int castleType = -1;

	for (unsigned int row = 0; row < ROW_COUNT; row++) {
		for (unsigned int col = 0; col < COL_COUNT; col++) {
			if (gridTiles[row][col].GetSprite().getGlobalBounds().contains(activePiece->GetSprite().getPosition().x, activePiece->GetSprite().getPosition().y)) {
				CheckMoveCounterReset(row, col);
				isCapture = gridPieces[row][col] != nullptr; //T = capture, F = regular move
				if (row == originalRowIndex && col == originalColIndex) {
					//no changes were made to the board state
				}
				else if (activePiece->CanCastle()) {
					if (activePiece->CanCastleLeft() && (col == 2)) { //The king has castled queenside
						isCastle = true;
						castleType = 1;
						//Move the king, knowing there is nothing there that needs to be deleted
						gridPieces[row][col] = activePiece;
						gridPieces[originalRowIndex][originalColIndex] = nullptr;

						//Move the left rook
						delete gridPieces[row][col - 2];
						gridPieces[row][col - 2] = nullptr;
						gridPieces[row][col + 1] = new Rook(activePiece->GetColor());
						gridPieces[row][col + 1]->GetSprite().setPosition(gridTiles[row][col + 1].GetPosition());
						gridPieces[row][col + 1]->SetMoveCount(1);
					}
					else if (activePiece->CanCastleRight() && (col == 6)) { //The king has castled kingside
						isCastle = true;
						castleType = 0;
						//Move the king, knowing there is nothing there that needs to be deleted
						gridPieces[row][col] = activePiece;
						gridPieces[originalRowIndex][originalColIndex] = nullptr;

						//Move the right rook
						delete gridPieces[row][col + 1];
						gridPieces[row][col + 1] = nullptr;
						gridPieces[row][col - 1] = new Rook(activePiece->GetColor());
						gridPieces[row][col - 1]->GetSprite().setPosition(gridTiles[row][col - 1].GetPosition());
						gridPieces[row][col - 1]->SetMoveCount(1);
					}
					else {
						//Move like normal
						delete gridPieces[row][col];
						gridPieces[row][col] = nullptr;
						gridPieces[row][col] = activePiece;
						gridPieces[originalRowIndex][originalColIndex] = nullptr;
					}
					UpdateWindow(window); //Force the visual update of the rook
				}
				else if (activePiece->CanEnPassant()) {
					if (!activePiece->GetColor()) { //Black's En Passant
						isEnPassant = true;
						isCapture = true;
						delete gridPieces[row - 1][col];
						gridPieces[row - 1][col] = nullptr;
						gridPieces[row][col] = activePiece;
						gridPieces[originalRowIndex][originalColIndex] = nullptr;
					}
					else { //White's En Passant
						isEnPassant = true;
						isCapture = true;
						delete gridPieces[row + 1][col];
						gridPieces[row + 1][col] = nullptr;
						gridPieces[row][col] = activePiece;
						gridPieces[originalRowIndex][originalColIndex] = nullptr;
					}
				}
				else if ((row == 0 || row == 7) && activePiece->GetName() == "Pawn") { //Pawn Promotion
					isPromotion = true;
					srand(time(0));

					bool randomColor = (rand() % 100 + 1) <= 50 ? true : false;
					int randomType = rand() % 100 + 1; //Random integer [1, 100]

					delete gridPieces[row][col];
					gridPieces[row][col] = nullptr;
					if (randomType <= 25) {
						gridPieces[row][col] = new Knight(randomColor);
						promotionType = 0;
					}
					else if (randomType <= 50) {
						gridPieces[row][col] = new Bishop(randomColor);
						promotionType = 1;
					}
					else if (randomType <= 75) {
						gridPieces[row][col] = new Rook(randomColor);
						promotionType = 2;
					}
					else {
						gridPieces[row][col] = new Queen(randomColor);
						promotionType = 3;
					}

					gridPieces[row][col]->GetSprite().setPosition(gridTiles[row][col].GetPosition());
					gridPieces[originalRowIndex][originalColIndex] = nullptr;
					TrackAllPieces(); //A piece was added, retrack everything
				}
				else {
					delete gridPieces[row][col];
					gridPieces[row][col] = nullptr;
					gridPieces[row][col] = activePiece;
					gridPieces[originalRowIndex][originalColIndex] = nullptr;
				}
				AddLog(activePiece->GetName(), originalRowIndex, originalColIndex, row, col);
				AddAlgebraicLog(activePiece->GetName(), originalRowIndex, originalColIndex, row, col, isCapture, isPromotion, promotionType, isCastle, castleType, isEnPassant);
				return;
			}
		}
	}
}

void Board::UpdateWindow(sf::RenderWindow& window) {
	window.clear();
	Board::DrawBoard(window);
	Board::DrawPieces(window);
	window.display();
}

void Board::OverlayDots(sf::RenderWindow& window) {
	GetMovementOptions(); //Recalculate the dots for whatever was clicked
	UpdateWindow(window); //Clears the dots each time to prevent lingering sprites
	DisplayMovementOptions(window); //Show the dots
	window.display();
}

void Board::AddLog(string name, int startRow, int startCol, int endRow, int endCol) {
	logs.push_back(Log(name, startRow, startCol, endRow, endCol));
}

void Board::AddAlgebraicLog(string name, int startRow, int startCol, int endRow, int endCol, bool isCapture, bool isPromotion, int promotionType, bool isCastle, int castleType, bool isEnPassant) {
	algebraicLogs.push_back(AlgebraicLog(name, startRow, startCol, endRow, endCol, isCapture, isPromotion, promotionType, isCastle, castleType, isEnPassant));
}

int Board::GetGameState() {
	return gameState;
}

ChessPiece* Board::GetActivePiece() {
	return activePiece;
}

vector<Log>& Board::GetLogs() {
	return logs;
}

vector<AlgebraicLog>& Board::GetAlgebraicLogs() {
	return algebraicLogs;
}

int Board::ConvertPxToIndx(float px) {
	//0->0, 96->1 ... 768->7
	return (int)px / 96;
}

//Cleanup things
Board::~Board() {
	for (unsigned int i = 0; i < gridPieces.size(); i++) {
		for(unsigned int j = 0; j < gridPieces[i].size(); j++) {
			delete gridPieces[i][j];
			gridPieces[i][j] = nullptr;
		}
	}
}

//DEBUG
void Board::DisplayPieceGrid() {
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if (gridPieces[i][j] == nullptr) {
				cout << "null  ";
			}
			else {
				gridPieces[i][j]->PrintName();
			}
		}
		cout << endl;
	}
}

void Board::DisplayMovesLists() {
	cout << "WHITE'S LEGAL MOVES: " << whitePieces.size() << " PIECES REMAINING" << endl;
	for(unsigned int i = 0; i < whitePieces.size(); i++) {
		for (unsigned int j = 0; j < whitePieces[i]->GetLegalMoves().size(); j++) {
			cout << "(" << whitePieces[i]->GetLegalMoves()[j].x << ", " << whitePieces[i]->GetLegalMoves()[j].y << ")    ";
		}
		cout << endl;
		cout << endl;
	}

	cout << "WHITE'S LEGAL MOVES WITH FF: " << whitePieces.size() << " PIECES REMAINING" << endl;
	for (unsigned int i = 0; i < whitePieces.size(); i++) {
		for (unsigned int j = 0; j < whitePieces[i]->GetLegalMovesWithFF().size(); j++) {
			cout << "(" << whitePieces[i]->GetLegalMovesWithFF()[j].x << ", " << whitePieces[i]->GetLegalMovesWithFF()[j].y << ")    ";
		}
		cout << endl;
		cout << endl;
	}

	cout << "WHITE'S TOTAL MOVES: " << whitePieces.size() << " PIECES REMAINING" << endl;
	for (unsigned int i = 0; i < whitePieces.size(); i++) {
		for (unsigned int j = 0; j < whitePieces[i]->GetTotalMoves().size(); j++) {
			cout << "(" << whitePieces[i]->GetTotalMoves()[j].x << ", " << whitePieces[i]->GetTotalMoves()[j].y << ")    ";
		}
		cout << endl;
		cout << endl;
	}

	cout << endl;
	cout << "BLACK'S LEGAL MOVES: " << blackPieces.size() << " PIECES REMAINING" << endl;
	for (unsigned int i = 0; i < blackPieces.size(); i++) {
		for (unsigned int j = 0; j < blackPieces[i]->GetLegalMoves().size(); j++) {
			cout << "(" << blackPieces[i]->GetLegalMoves()[j].x << ", " << blackPieces[i]->GetLegalMoves()[j].y << ")    ";
		}
		cout << endl;
		cout << endl;
	}

	cout << endl;
	cout << "BLACK'S LEGAL MOVES INCLUDING FF: " << blackPieces.size() << " PIECES REMAINING" << endl;
	for (unsigned int i = 0; i < blackPieces.size(); i++) {
		for (unsigned int j = 0; j < blackPieces[i]->GetLegalMovesWithFF().size(); j++) {
			cout << "(" << blackPieces[i]->GetLegalMovesWithFF()[j].x << ", " << blackPieces[i]->GetLegalMovesWithFF()[j].y << ")    ";
		}
		cout << endl;
		cout << endl;
	}

	cout << "BLACK'S TOTAL MOVES: " << blackPieces.size() << " PIECES REMAINING" << endl;
	for (unsigned int i = 0; i < blackPieces.size(); i++) {
		for (unsigned int j = 0; j < blackPieces[i]->GetTotalMoves().size(); j++) {
			cout << "(" << blackPieces[i]->GetTotalMoves()[j].x << ", " << blackPieces[i]->GetTotalMoves()[j].y << ")    ";
		}
		cout << endl;
		cout << endl;
	}
}

void Board::DisplayKingCheck() {
	cout << "BOARD STATES: " << endl;
	cout << "White in check: " << whiteInCheck << endl;
	cout << "Black in check: " << blackInCheck << endl;

	cout << "PIECE STATES: " << endl;
	whiteKing->PrintCheckState();
	blackKing->PrintCheckState();
}

void Board::DisplayAttackingPieces() {
	cout << "WHITE PIECES ATTACKING KING: " << endl;
	for (unsigned int i = 0; i < whitePieces.size(); i++) {
		if (whitePieces[i]->IsAttackingKing()) {
			cout << "(" << whitePieces[i]->GetSprite().getPosition().x << ", " << whitePieces[i]->GetSprite().getPosition().y << ")" << endl;
		}
	}

	cout << "BLACK PIECES ATTACKING KING: " << endl;
	for (unsigned int i = 0; i < blackPieces.size(); i++) {
		if (blackPieces[i]->IsAttackingKing()) {
			cout << "(" << blackPieces[i]->GetSprite().getPosition().x << ", " << blackPieces[i]->GetSprite().getPosition().y << ")" << endl;
		}
	}
}

void Board::ShowDotListSize() {
	cout << "There are " << gridDots.size() << " dots on the screen." << endl;
}

void Board::PrintMoveCounter() {
	cout << moveCounter << " moves since a pawn moved or piece was captured." << endl;
}

void Board::PrintLogs() {
	for (unsigned int i = 0; i < logs.size(); i++) {
		logs[i].PrintLog();
	}
}

void Board::PrintAlgebraicLogs() {
	for (unsigned int i = 0; i < algebraicLogs.size(); i++) {
		algebraicLogs[i].PrintLog();
	}
}

void Board::PrintProtectingPieces() {
	for (unsigned int i = 0; i < blackPieces.size(); i++) {
		if (blackPieces[i]->IsProtecting()) {
			cout << blackPieces[i]->GetSprite().getPosition().x << ", " << blackPieces[i]->GetSprite().getPosition().y << endl;
		}
	}
}