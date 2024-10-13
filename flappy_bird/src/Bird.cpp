#include "Bird.hpp"

#include "Config.hpp"
#include <SFML/Graphics.hpp>

Bird::Bird(const AssetManager &assetManager) {
    const auto &texture{assetManager.getTexture("yellowbird-midflap")};

    m_sprite.setTexture(texture, true);
    m_sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

void Bird::update(float deltaTime) {
    m_velocity += GRAVITY;
    m_sprite.move(0, m_velocity * deltaTime);
}

void Bird::draw(sf::RenderTarget &target) { target.draw(m_sprite); }

void Bird::jump() { m_velocity = -JUMP_STRENGTH; }

sf::FloatRect Bird::getBounds() const { return m_sprite.getGlobalBounds(); }

void Bird::setPosition(const sf::Vector2f &position) {
    m_sprite.setPosition(position);
}

sf::Vector2f Bird::getPosition() const { return m_sprite.getPosition(); }

void Bird::setScale(const sf::Vector2f &scale) { m_sprite.setScale(scale); }

sf::Vector2f Bird::getScale() const { return m_sprite.getScale(); }

void Bird::setVelocity(float velocity) { m_velocity = velocity; }
