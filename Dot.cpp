#include "Dot.h"

Dot::Dot() {
	dotSprite.setTexture(TextureManager::GetTexture("dot"));
}

sf::Sprite& Dot::GetSprite() {
	return dotSprite;
}

void Dot::Draw(sf::RenderWindow& window) {
	window.draw(dotSprite);
}