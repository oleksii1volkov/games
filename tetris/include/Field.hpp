#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class EventManager;
class Tetromino;

class Field {
public:
    Field(const sf::Vector2f &position, const sf::Vector2f &cellSize,
          const sf::Vector2u &gridSize,
          const std::shared_ptr<EventManager> &eventManager);

    void draw(sf::RenderTarget &target) const;

    void setPosition(const sf::Vector2f &position);
    const sf::Vector2f &getPosition() const;

    void setCellSize(const sf::Vector2f &cellSize);
    const sf::Vector2f &getCellSize() const;

    void addTetromino(std::unique_ptr<Tetromino> tetromino);
    const std::unique_ptr<Tetromino> &getTetromino() const;

    void clear();

    void rotateTetrominoClockwise();
    void rotateTetrominoCounterClockwise();
    void moveTetrominoLeft();
    void moveTetrominoRight();
    void moveTetrominoDown();
    void dropTetromino();

private:
    using TetrominoShape = std::vector<std::string>;

    bool canMoveDownTetromino(const TetrominoShape &tetrominoShape,
                              const sf::Vector2i &tetrominoPosition) const;
    bool canMoveLeftTetromino(const TetrominoShape &tetrominoShape,
                              const sf::Vector2i &tetrominoPosition) const;
    bool canMoveRightTetromino(const TetrominoShape &tetrominoShape,
                               const sf::Vector2i &tetrominoPosition) const;
    bool
    canRotateClockwiseTetromino(const TetrominoShape &tetrominoShape,
                                const sf::Vector2i &tetrominoPosition) const;
    bool canRotateCounterClockwiseTetromino(
        const TetrominoShape &tetrominoShape,
        const sf::Vector2i &tetrominoPosition) const;
    bool intersectsWithTetrominos(const TetrominoShape &tetrominoShape,
                                  const sf::Vector2i &tetrominoPosition) const;
    void adjustTetrominoPosition(const TetrominoShape &tetrominoShape,
                                 sf::Vector2i &tetrominoPosition) const;

    void placeTetromino();
    void updateTetrominoCellsPosition();
    void eliminateRows();
    void shiftRowsAbove(int rowIndex);

private:
    sf::Vector2f m_position;
    sf::Vector2f m_cellSize;
    sf::Vector2u m_gridSize;

    std::vector<std::string> m_grid;
    std::vector<std::vector<std::optional<sf::RectangleShape>>> m_cells;

    std::vector<sf::RectangleShape> m_backgroundGrid;
    sf::RectangleShape m_background;

    std::unique_ptr<Tetromino> m_tetromino;
    sf::Vector2i m_tetrominoPosition;

    std::shared_ptr<EventManager> m_eventManager;
};
