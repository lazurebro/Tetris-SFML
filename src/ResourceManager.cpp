#include "ResourceManager.h"

std::unordered_map<std::string, sf::Texture> ResourceManager::textures{};

sf::Texture* ResourceManager::getTexture(const std::string& texture_path)
{
	if (textures.find(texture_path) == textures.end())
	{
		sf::Texture new_texture;
		new_texture.loadFromFile(texture_path);

		textures[texture_path] = std::move(new_texture);
	}

	return &(textures[texture_path]);
}