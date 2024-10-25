#include "Field.hpp"

#include "EventManager.hpp"
#include "Tetromino.hpp"

#include <iostream>
#include <iterator>
#include <stdexcept>

Field::Field(const sf::Vector2f &position, const sf::Vector2f &cellSize,
             const sf::Vector2u &gridSize,
             const std::shared_ptr<EventManager> &eventManager)
    : m_position{position}, m_cellSize{cellSize}, m_gridSize{gridSize},
      m_eventManager{eventManager} {
    m_background.setSize(
        sf::Vector2f{gridSize.x * cellSize.x, gridSize.y * cellSize.y});
    m_background.setPosition(m_position);
    m_background.setFillColor(sf::Color{244, 250, 255});
    m_background.setOutlineColor(sf::Color::Transparent);
    m_background.setOutlineThickness(0);

    for (size_t rowIndex{0}; rowIndex < gridSize.y; ++rowIndex) {
        for (size_t columnIndex{0}; columnIndex < gridSize.x; ++columnIndex) {
            sf::RectangleShape cell;
            cell.setSize(cellSize);
            cell.setPosition(m_position + sf::Vector2f{cellSize.x * columnIndex,
                                                       cellSize.y * rowIndex});
            cell.setFillColor(sf::Color::Transparent);
            cell.setOutlineColor(sf::Color{217, 217, 213});
            cell.setOutlineThickness(0);

            m_backgroundGrid.emplace_back(std::move(cell));
        }
    }

    for (size_t rowIndex{0}; rowIndex < gridSize.y; ++rowIndex) {
        m_grid.emplace_back(std::string(gridSize.x, ' '));
        m_cells.emplace_back(std::vector<std::optional<sf::RectangleShape>>(
            gridSize.x, std::nullopt));
    }
}

void Field::draw(sf::RenderTarget &target) const {
    target.draw(m_background);

    for (const auto &cell : m_backgroundGrid) {
        target.draw(cell);
    }

    for (const auto &row : m_cells) {
        for (const auto &cell : row) {
            if (cell) {
                target.draw(*cell);
            }
        }
    }

    if (m_tetromino) {
        m_tetromino->draw(target);
    }
}

void Field::setPosition(const sf::Vector2f &position) { m_position = position; }

const sf::Vector2f &Field::getPosition() const { return m_position; }

void Field::setCellSize(const sf::Vector2f &cellSize) { m_cellSize = cellSize; }

const sf::Vector2f &Field::getCellSize() const { return m_cellSize; }

void Field::addTetromino(std::unique_ptr<Tetromino> tetromino) {
    if (!tetromino) {
        return;
    }

    if (m_tetromino) {
        throw std::runtime_error{"Tetromino already exists"};
    }

    const sf::Vector2i tetrominoPosition{
        static_cast<int>(m_gridSize.x / 2 -
                         tetromino->getShape().front().size() / 2),
        0};

    if (intersectsWithTetrominos(tetromino->getShape(), tetrominoPosition)) {
        if (m_eventManager) {
            m_eventManager->publish("FieldIsFull", std::nullopt);
        }
    } else {
        m_tetromino = std::move(tetromino);
        m_tetrominoPosition = tetrominoPosition;

        updateTetrominoCellsPosition();
    }
}

const std::unique_ptr<Tetromino> &Field::getTetromino() const {
    return m_tetromino;
}

void Field::clear() {
    m_tetromino = nullptr;
    m_tetrominoPosition = sf::Vector2i{0, 0};

    for (auto &row : m_grid) {
        std::fill(row.begin(), row.end(), ' ');
    }

    for (auto &row : m_cells) {
        std::fill(row.begin(), row.end(), std::nullopt);
    }
}

namespace {
void printGrid(const std::vector<std::string> &grid,
               const std::unique_ptr<Tetromino> &tetromino,
               const sf::Vector2i &tetrominoPosition) {
    for (size_t rowIndex{0}; rowIndex < grid.size(); ++rowIndex) {
        for (size_t columnIndex{0}; columnIndex < grid[rowIndex].size();
             ++columnIndex) {
            if (tetromino && rowIndex >= tetrominoPosition.y &&
                rowIndex < tetrominoPosition.y + tetromino->getShape().size() &&
                static_cast<int>(columnIndex) >= tetrominoPosition.x &&
                static_cast<int>(columnIndex) <
                    tetrominoPosition.x + tetromino->getShape()[0].size()) {
                std::cout
                    << tetromino->getShape()[rowIndex - tetrominoPosition.y]
                                            [columnIndex - tetrominoPosition.x];
            } else {
                std::cout << grid[rowIndex][columnIndex];
            }
        }

        std::cout << std::endl;
    }

    std::cout << tetrominoPosition.x << ' ' << tetrominoPosition.y << std::endl;
}
} // namespace

void Field::moveTetrominoLeft() {
    if (!m_tetromino) {
        return;
    }

    if (!canMoveLeftTetromino(m_tetromino->getShape(), m_tetrominoPosition)) {
        return;
    }

    m_tetromino->setPosition(m_tetromino->getPosition() -
                             sf::Vector2f{m_cellSize.x, 0});
    m_tetrominoPosition.x -= 1;

    // printGrid(m_grid, m_tetromino, m_tetrominoPosition);
}

void Field::moveTetrominoRight() {
    if (!m_tetromino) {
        return;
    }

    if (!canMoveRightTetromino(m_tetromino->getShape(), m_tetrominoPosition)) {
        return;
    }

    m_tetromino->setPosition(m_tetromino->getPosition() +
                             sf::Vector2f{m_cellSize.x, 0});
    m_tetrominoPosition.x += 1;

    // printGrid(m_grid, m_tetromino, m_tetrominoPosition);
}

void Field::moveTetrominoDown() {
    if (!m_tetromino) {
        return;
    }

    if (!canMoveDownTetromino(m_tetromino->getShape(), m_tetrominoPosition)) {
        placeTetromino();
    } else {
        m_tetromino->setPosition(m_tetromino->getPosition() +
                                 sf::Vector2f{0, m_cellSize.y});
        m_tetrominoPosition.y += 1;
    }

    // printGrid(m_grid, m_tetromino, m_tetrominoPosition);
}

void Field::dropTetromino() {}

void Field::rotateTetrominoClockwise() {
    if (!m_tetromino) {
        return;
    }

    if (!canRotateClockwiseTetromino(m_tetromino->getShape(),
                                     m_tetrominoPosition)) {
        return;
    }

    m_tetromino->rotateClockwise();
    adjustTetrominoPosition(m_tetromino->getShape(), m_tetrominoPosition);
    updateTetrominoCellsPosition();

    // printGrid(m_grid, m_tetromino, m_tetrominoPosition);
}

void Field::rotateTetrominoCounterClockwise() {
    if (!m_tetromino) {
        return;
    }

    if (!canRotateCounterClockwiseTetromino(m_tetromino->getShape(),
                                            m_tetrominoPosition)) {
        return;
    }

    m_tetromino->rotateCounterClockwise();
    adjustTetrominoPosition(m_tetromino->getShape(), m_tetrominoPosition);
    updateTetrominoCellsPosition();

    // printGrid(m_grid, m_tetromino, m_tetrominoPosition);
}

bool Field::canMoveDownTetromino(const TetrominoShape &tetrominoShape,
                                 const sf::Vector2i &tetrominoPosition) const {
    for (std::size_t rowIndex{0}; rowIndex < tetrominoShape.size();
         ++rowIndex) {
        for (std::size_t columnIndex{0};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            if (tetrominoShape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            if (tetrominoPosition.y + rowIndex + 1 >= m_gridSize.y) {
                return false;
            }

            if (m_grid[tetrominoPosition.y + rowIndex + 1]
                      [tetrominoPosition.x + columnIndex] != ' ') {
                return false;
            }
        }
    }

    return true;
}

bool Field::canMoveLeftTetromino(const TetrominoShape &tetrominoShape,
                                 const sf::Vector2i &tetrominoPosition) const {
    for (std::size_t rowIndex{0}; rowIndex < tetrominoShape.size();
         ++rowIndex) {
        for (std::size_t columnIndex{0};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            if (tetrominoShape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            if (tetrominoPosition.x + static_cast<int>(columnIndex) - 1 < 0) {
                return false;
            }

            if (m_grid[tetrominoPosition.y + rowIndex]
                      [tetrominoPosition.x + columnIndex - 1] != ' ') {
                return false;
            }
        }
    }

    return true;
}

bool Field::canMoveRightTetromino(const TetrominoShape &tetrominoShape,
                                  const sf::Vector2i &tetrominoPosition) const {
    for (std::size_t rowIndex{0}; rowIndex < tetrominoShape.size();
         ++rowIndex) {
        for (std::size_t columnIndex{0};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            if (tetrominoShape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            if (tetrominoPosition.x + columnIndex + 1 >= m_gridSize.x) {
                return false;
            }

            if (m_grid[tetrominoPosition.y + rowIndex]
                      [tetrominoPosition.x + columnIndex + 1] != ' ') {
                return false;
            }
        }
    }

    return true;
}

namespace {
void rotateClockwiseTetrominoShape(std::vector<std::string> &tetrominoShape) {
    for (size_t rowIndex{0}; rowIndex < tetrominoShape.size(); ++rowIndex) {
        for (size_t columnIndex{rowIndex + 1};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            std::swap(tetrominoShape[rowIndex][columnIndex],
                      tetrominoShape[columnIndex][rowIndex]);
        }
    }

    for (size_t rowIndex{0}; rowIndex < tetrominoShape.size(); ++rowIndex) {
        std::reverse(tetrominoShape[rowIndex].begin(),
                     tetrominoShape[rowIndex].end());
    }
}

void rotateCounterClockwiseTetrominoShape(
    std::vector<std::string> &tetrominoShape) {
    for (size_t rowIndex{0}; rowIndex < tetrominoShape.size(); ++rowIndex) {
        for (size_t columnIndex{rowIndex + 1};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            std::swap(tetrominoShape[rowIndex][columnIndex],
                      tetrominoShape[columnIndex][rowIndex]);
        }
    }

    for (size_t columnIndex{0}; columnIndex < tetrominoShape.size();
         ++columnIndex) {
        for (size_t rowIndex{0}; rowIndex < tetrominoShape.size() / 2;
             ++rowIndex) {
            std::swap(tetrominoShape[rowIndex][columnIndex],
                      tetrominoShape[tetrominoShape.size() - rowIndex - 1]
                                    [columnIndex]);
        }
    }
}
} // namespace

bool Field::canRotateClockwiseTetromino(
    const TetrominoShape &tetrominoShape,
    const sf::Vector2i &tetrominoPosition) const {
    auto rotatedTetrominoShape{tetrominoShape};
    auto rotatedTetrominoPosition{tetrominoPosition};

    rotateClockwiseTetrominoShape(rotatedTetrominoShape);
    adjustTetrominoPosition(rotatedTetrominoShape, rotatedTetrominoPosition);

    return !intersectsWithTetrominos(rotatedTetrominoShape,
                                     rotatedTetrominoPosition);
}

bool Field::canRotateCounterClockwiseTetromino(
    const TetrominoShape &tetrominoShape,
    const sf::Vector2i &tetrominoPosition) const {
    auto rotatedTetrominoShape{tetrominoShape};
    auto rotatedTetrominoPosition{tetrominoPosition};

    rotateCounterClockwiseTetrominoShape(rotatedTetrominoShape);
    adjustTetrominoPosition(rotatedTetrominoShape, rotatedTetrominoPosition);

    return !intersectsWithTetrominos(rotatedTetrominoShape,
                                     rotatedTetrominoPosition);
}

bool Field::intersectsWithTetrominos(
    const TetrominoShape &tetrominoShape,
    const sf::Vector2i &tetrominoPosition) const {
    for (std::size_t rowIndex{0}; rowIndex < tetrominoShape.size();
         ++rowIndex) {
        for (std::size_t columnIndex{0};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            if (tetrominoShape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            if (tetrominoPosition.x + static_cast<int>(columnIndex) < 0 ||
                tetrominoPosition.x + static_cast<int>(columnIndex) >=
                    m_gridSize.x) {
                continue;
            }

            if (m_grid[tetrominoPosition.y + rowIndex]
                      [tetrominoPosition.x + columnIndex] != ' ') {
                return true;
            }
        }
    }

    return false;
}

void Field::adjustTetrominoPosition(const TetrominoShape &tetrominoShape,
                                    sf::Vector2i &tetrominoPosition) const {
    for (std::size_t rowIndex{0}; rowIndex < tetrominoShape.size();
         ++rowIndex) {
        for (std::size_t columnIndex{0};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            if (tetrominoShape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            const int xCellPosition{tetrominoPosition.x +
                                    static_cast<int>(columnIndex)};

            if (xCellPosition < 0) {
                tetrominoPosition.x += -xCellPosition;
            } else if (xCellPosition >= m_gridSize.x) {
                tetrominoPosition.x -= xCellPosition - m_gridSize.x + 1;
            }
        }
    }
}

void Field::placeTetromino() {
    if (!m_tetromino) {
        return;
    }

    auto tetrominoCells{std::move(*m_tetromino).getCells()};
    auto tetrominoCellIter{tetrominoCells.begin()};
    const auto &tetrominoShape{m_tetromino->getShape()};

    for (std::size_t rowIndex{0}; rowIndex < tetrominoShape.size();
         ++rowIndex) {
        for (std::size_t columnIndex{0};
             columnIndex < tetrominoShape[rowIndex].size(); ++columnIndex) {
            if (tetrominoShape[rowIndex][columnIndex] == ' ') {
                continue;
            }

            m_grid[m_tetrominoPosition.y + rowIndex]
                  [m_tetrominoPosition.x + columnIndex] =
                      tetrominoShape[rowIndex][columnIndex];

            m_cells[m_tetrominoPosition.y + rowIndex]
                   [m_tetrominoPosition.x + columnIndex] =
                       std::move(*tetrominoCellIter++);
        }
    }

    m_tetromino.reset();

    eliminateRows();

    if (m_eventManager) {
        m_eventManager->publish("TetrominoPlaced", std::nullopt);
    }
}

void Field::updateTetrominoCellsPosition() {
    if (!m_tetromino) {
        return;
    }

    m_tetromino->setPosition(
        m_position + sf::Vector2f{m_cellSize.x * m_tetrominoPosition.x,
                                  m_cellSize.y * m_tetrominoPosition.y});
}

void Field::eliminateRows() {
    size_t rowsEliminated{0};

    for (std::size_t rowIndex{0}; rowIndex < m_gridSize.y; ++rowIndex) {
        if (m_grid[rowIndex].find(' ') == std::string::npos) {
            ++rowsEliminated;

            shiftRowsAbove(rowIndex);

            m_grid.erase(m_grid.begin() + rowIndex);
            m_grid.insert(m_grid.begin(), std::string(m_gridSize.x, ' '));

            m_cells.erase(m_cells.begin() + rowIndex);
            m_cells.insert(m_cells.begin(),
                           std::vector<std::optional<sf::RectangleShape>>(
                               m_gridSize.x, std::nullopt));
        }
    }

    if (rowsEliminated > 0) {
        if (m_eventManager) {
            m_eventManager->publish("RowsEliminated", rowsEliminated);
        }
    }
}

void Field::shiftRowsAbove(int rowIndex) {
    bool emptyRow{true};
    --rowIndex;

    for (; rowIndex >= 0; --rowIndex) {
        for (std::size_t columnIndex{0}; columnIndex < m_gridSize.x;
             ++columnIndex) {
            if (m_cells[rowIndex][columnIndex] != std::nullopt) {
                emptyRow = false;

                auto &cell{m_cells[rowIndex][columnIndex].value()};
                cell.setPosition(cell.getPosition() +
                                 sf::Vector2f{0, m_cellSize.y});
            }
        }

        if (emptyRow) {
            break;
        }
    }
}
