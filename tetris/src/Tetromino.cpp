#include "Tetromino.hpp"

#include <iostream>

Tetromino::Tetromino(const sf::Vector2f &cellSize,
                     const std::vector<std::string> &shape)
    : m_position{}, m_cellSize{cellSize}, m_shape{shape} {
    rebuild();
}

void Tetromino::rebuild() {
    m_cells.clear();

    for (std::size_t rowIndex{0}; rowIndex < m_shape.size(); ++rowIndex) {
        for (std::size_t columnIndex{0}; columnIndex < m_shape[rowIndex].size();
             ++columnIndex) {
            if (m_shape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            sf::RectangleShape cell;
            cell.setSize(m_cellSize);
            cell.setPosition(m_position +
                             sf::Vector2f{m_cellSize.x * columnIndex,
                                          m_cellSize.y * rowIndex});
            cell.setFillColor(sf::Color{244, 96, 54});
            cell.setOutlineColor(sf::Color{217, 217, 213});
            cell.setOutlineThickness(2);

            m_cells.emplace_back(std::move(cell));
        }
    }
}

void Tetromino::rotateClockwise() {
    for (size_t rowIndex{0}; rowIndex < m_shape.size(); ++rowIndex) {
        for (size_t columnIndex{rowIndex + 1};
             columnIndex < m_shape[rowIndex].size(); ++columnIndex) {
            std::swap(m_shape[rowIndex][columnIndex],
                      m_shape[columnIndex][rowIndex]);
        }
    }

    for (size_t rowIndex{0}; rowIndex < m_shape.size(); ++rowIndex) {
        std::reverse(m_shape[rowIndex].begin(), m_shape[rowIndex].end());
    }

    rebuild();
}

void Tetromino::rotateCounterClockwise() {
    for (size_t rowIndex{0}; rowIndex < m_shape.size(); ++rowIndex) {
        for (size_t columnIndex{rowIndex + 1};
             columnIndex < m_shape[rowIndex].size(); ++columnIndex) {
            std::swap(m_shape[rowIndex][columnIndex],
                      m_shape[columnIndex][rowIndex]);
        }
    }

    for (size_t columnIndex{0}; columnIndex < m_shape.size(); ++columnIndex) {
        for (size_t rowIndex{0}; rowIndex < m_shape.size() / 2; ++rowIndex) {
            std::swap(m_shape[rowIndex][columnIndex],
                      m_shape[m_shape.size() - rowIndex - 1][columnIndex]);
        }
    }

    rebuild();
}

void Tetromino::draw(sf::RenderTarget &target) const {
    for (const auto &cell : m_cells) {
        target.draw(cell);
    }
}

void Tetromino::setPosition(const sf::Vector2f &position) {
    const auto distance{position - m_position};

    for (auto &cell : m_cells) {
        cell.setPosition(cell.getPosition() + distance);
    }

    m_position = position;
}

const sf::Vector2f &Tetromino::getPosition() const { return m_position; }

const std::vector<std::string> &Tetromino::getShape() const { return m_shape; }

std::vector<sf::RectangleShape> Tetromino::getCells() && {
    return std::move(m_cells);
}
