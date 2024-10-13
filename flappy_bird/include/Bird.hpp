#pragma once

#include "AssetManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Bird {
public:
    Bird(const AssetManager &assetManager);

    void update(float deltaTime);
    void draw(sf::RenderTarget &target);
    void jump();

    sf::FloatRect getBounds() const;

    void setPosition(const sf::Vector2f &position);
    sf::Vector2f getPosition() const;

    void setScale(const sf::Vector2f &scale);
    sf::Vector2f getScale() const;

    void setVelocity(float velocity);

private:
    sf::Sprite m_sprite;
    float m_velocity{0.0f};
};
