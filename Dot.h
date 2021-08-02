#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Dot {
private:
	sf::Sprite dotSprite;

public:
	Dot();
	sf::Sprite& GetSprite();
	void Draw(sf::RenderWindow& window);
};

