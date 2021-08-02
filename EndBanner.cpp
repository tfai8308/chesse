#include "EndBanner.h"

sf::Sprite EndBanner::endBanner;

void EndBanner::LoadBanner(string name) {
	endBanner.setTexture(TextureManager::GetTexture(name));
	endBanner.setPosition(0, 192);
}

void EndBanner::DrawBanner(int gameResult, sf::RenderWindow& window) {
	switch (gameResult) {
	case 0:
		LoadBanner("game_draw");
		break;
	case 1:
		LoadBanner("white_victory");
		break;
	case 2:
		LoadBanner("black_victory");
		break;
	default:
		LoadBanner("missing_banner");
		break;
	}

	Board::UpdateWindow(window); //Update the piece movement first
	window.draw(endBanner);
	window.display();
}