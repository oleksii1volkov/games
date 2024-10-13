#include "AssetManager.hpp"

#include <stdexcept>

AssetManager::AssetManager() {
    auto loadTexture = [](const std::string &texturePath) {
        sf::Texture texture;

        if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }

        return texture;
    };

    m_textures = {
        {"pipe-green", loadTexture("resources/sprites/pipe-green.png")},
        {"yellowbird-midflap",
         loadTexture("resources/sprites/yellowbird-midflap.png")},
        {"background", loadTexture("resources/sprites/background-day.png")},
        {"0", loadTexture("resources/sprites/0.png")},
        {"1", loadTexture("resources/sprites/1.png")},
        {"2", loadTexture("resources/sprites/2.png")},
        {"3", loadTexture("resources/sprites/3.png")},
        {"4", loadTexture("resources/sprites/4.png")},
        {"5", loadTexture("resources/sprites/5.png")},
        {"6", loadTexture("resources/sprites/6.png")},
        {"7", loadTexture("resources/sprites/7.png")},
        {"8", loadTexture("resources/sprites/8.png")},
        {"9", loadTexture("resources/sprites/9.png")},
    };
}

const sf::Texture &AssetManager::getTexture(const std::string &name) const {
    if (const auto textureIter{m_textures.find(name)};
        textureIter == m_textures.end()) {
        throw std::runtime_error("Texture not found: " + name);
    } else {
        return textureIter->second;
    }
}
