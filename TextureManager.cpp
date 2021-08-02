#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTextures(string fileName) {

	string path = "images/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName) {

	if (textures.find(textureName) == textures.end()) {
		LoadTextures(textureName);
	}

	return textures[textureName];
}

void TextureManager::Clear() {
	textures.clear();
}