#pragma once

#include "AssetManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Pipe {
public:
    Pipe(const AssetManager &assetManager);

    void update(float deltaTime);
    void draw(sf::RenderTarget &target);

    sf::FloatRect getBounds() const;

    void setPosition(const sf::Vector2f &position);
    sf::Vector2f getPosition() const;

    void setRotation(float rotation);
    float getRotation() const;

    void setScale(const sf::Vector2f &scale);
    sf::Vector2f getScale() const;

    void setOrigin(const sf::Vector2f &origin);
    sf::Vector2f getOrigin() const;

private:
    sf::Sprite m_sprite;
};
