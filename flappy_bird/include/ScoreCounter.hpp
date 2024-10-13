#pragma once

#include "AssetManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>

class ScoreCounter {
public:
    ScoreCounter(const AssetManager &assetManager);

    void draw(sf::RenderTarget &target);
    void setPosition(const sf::Vector2f &position);

    void increment();
    void reset();

private:
    const AssetManager &m_assetManager;
    size_t m_counter{0};
    std::vector<sf::Sprite> m_digitSprites;
    sf::Vector2f m_position;
};
