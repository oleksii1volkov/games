#include "Pipe.hpp"

#include "Config.hpp"

Pipe::Pipe(const AssetManager &assetManager) {
    const auto &texture{assetManager.getTexture("pipe-green")};

    m_sprite.setTexture(texture, true);
}

void Pipe::update(float deltaTime) {
    m_sprite.move(-PIPE_SPEED * deltaTime, 0);
}

void Pipe::draw(sf::RenderTarget &target) { target.draw(m_sprite); }

sf::FloatRect Pipe::getBounds() const { return m_sprite.getGlobalBounds(); }

void Pipe::setPosition(const sf::Vector2f &position) {
    m_sprite.setPosition(position);
}

sf::Vector2f Pipe::getPosition() const { return m_sprite.getPosition(); }

void Pipe::setRotation(float rotation) { m_sprite.setRotation(rotation); }

float Pipe::getRotation() const { return m_sprite.getRotation(); }

void Pipe::setScale(const sf::Vector2f &scale) { m_sprite.setScale(scale); }

sf::Vector2f Pipe::getScale() const { return m_sprite.getScale(); }

void Pipe::setOrigin(const sf::Vector2f &origin) { m_sprite.setOrigin(origin); }

sf::Vector2f Pipe::getOrigin() const { return m_sprite.getOrigin(); }
