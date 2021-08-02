#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Board.h"

using namespace std;

class EndBanner {
private:
	static sf::Sprite endBanner;

	static void LoadBanner(string name);

public:
	static void DrawBanner(int gameResult, sf::RenderWindow& window);
};

