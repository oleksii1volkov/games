#pragma once

#include <SFML/System/Vector2.hpp>

#include <memory>

class Tetromino;

class TetrominoFactory {
public:
    enum Type { I, J, L, O, S, T, Z, TETROMINOS_COUNT };

    TetrominoFactory();

    std::unique_ptr<Tetromino> createTetromino(const sf::Vector2f &cellSize,
                                               Type tetrominoType) const;
    std::unique_ptr<Tetromino>
    createRandomTetromino(const sf::Vector2f &cellSize) const;
};
