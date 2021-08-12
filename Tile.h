#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace std;

class Tile {
private:
	sf::Sprite tileSprite;

public:
	Tile();
	void SetTexture(string texture);
	void Draw(sf::RenderWindow& window);
	sf::Sprite& GetSprite();
	const sf::Vector2f& GetPosition();
};