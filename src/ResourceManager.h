#pragma once

#include "SFML/Graphics.hpp"
#include <unordered_map>
#include "string"

class ResourceManager
{
	static std::unordered_map<std::string, sf::Texture> textures;
public:
	static sf::Texture* getTexture(const std::string& texture_path);
};