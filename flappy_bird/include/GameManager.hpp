#pragma once

#include "AssetManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class GameManager {
public:
    GameManager(const AssetManager &assetManager);

    void run(sf::RenderWindow &window);

private:
    const AssetManager &m_assetManager;
};
