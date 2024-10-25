#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>
#include <string>
#include <vector>

class Tetromino {
public:
    Tetromino(const sf::Vector2f &cellSize,
              const std::vector<std::string> &shape);

    void draw(sf::RenderTarget &target) const;

    void rotateClockwise();
    void rotateCounterClockwise();

    void setPosition(const sf::Vector2f &position);
    const sf::Vector2f &getPosition() const;

    const std::vector<std::string> &getShape() const;
    std::vector<sf::RectangleShape> getCells() &&;

private:
    void rebuild();

private:
    sf::Vector2f m_position;
    sf::Vector2f m_cellSize;
    std::vector<std::string> m_shape;
    std::vector<sf::RectangleShape> m_cells;
};
