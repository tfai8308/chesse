#include "Tile.h"

Tile::Tile() {
	tileSprite.setTexture(TextureManager::GetTexture("default_tile"));
}

void Tile::SetTexture(string texture) {
	tileSprite.setTexture(TextureManager::GetTexture(texture));
}

void Tile::Draw(sf::RenderWindow& window) {
	window.draw(tileSprite);
}

sf::Sprite& Tile::GetSprite() {
	return tileSprite;
}

const sf::Vector2f& Tile::GetPosition() {
	return tileSprite.getPosition();
}