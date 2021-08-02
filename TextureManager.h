#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

class TextureManager {
private:
	static unordered_map<string, sf::Texture> textures;
	static void LoadTextures(string fileName);

public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear();
};

