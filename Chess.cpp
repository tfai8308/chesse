#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"

using namespace std;

int main()
{
    bool verbose = false;
    Board::InitializeBoard();
    sf::RenderWindow window(sf::VideoMode(Board::TILE_SIZE * Board::COL_COUNT, Board::TILE_SIZE * Board::ROW_COUNT), "Chesse", sf::Style::Titlebar | sf::Style::Close);
    Board::UpdateWindow(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event) && Board::GetGameState() == Board::GameState::ACTIVE)
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
           
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int mouseX = mousePos.x;
            int mouseY = mousePos.y;
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Board::ClickEvent(window, mousePos, Board::pieceClicked);
                }
                if (verbose && event.mouseButton.button == sf::Mouse::Right) {
                    Board::DisplayPieceGrid(); //DEBUG
                    cout << endl;
                    Board::DisplayMovesLists(); //DEBUG
                    cout << endl;
                    Board::DisplayKingCheck(); //DEBUG
                    cout << endl;
                    Board::DisplayAttackingPieces(); //DEBUG
                    cout << endl;
                    Board::ShowDotListSize(); //DEBUG
                    cout << endl;
                    Board::PrintMoveCounter(); //DEBUG
                    cout << endl;
                    Board::PrintLogs(); //DEBUG
                    cout << endl;
                    Board::PrintAlgebraicLogs(); //DEBUG
                    cout << endl;
                    Board::PrintProtectingPieces(); //DEBUG
                    cout << endl;
                }
            }

            if (Board::pieceClicked && event.type == sf::Event::MouseButtonReleased) {
                Board::ReleasePiece(window, mousePos);
            }

            if (Board::pieceClicked && event.type == sf::Event::MouseMoved) {

                Board::GetActivePiece()->SetDragging(true); // 7/15 testing

                if (mouseX < 0 || mouseY < 0 || mouseX > window.getSize().x || mouseY > window.getSize().y) {
                    //The cursor has left the screen
                    Board::ResetPiecePosition(window);
                }
                else {
                    Board::DragEvent(window, mousePos);
                }
            }
        }
    }
    
    TextureManager::Clear();
    return 0;
}