#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <unordered_map>

class AssetManager {
public:
    AssetManager();

    const sf::Texture &getTexture(const std::string &name) const;

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
};
