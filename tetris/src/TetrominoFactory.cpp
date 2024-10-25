#include "TetrominoFactory.hpp"

#include "Tetromino.hpp"

namespace {
const std::vector<std::string> T_TETROMINO{" * ", "***", "   "};

const std::vector<std::string> S_TETROMINO{" **", "** ", "   "};

const std::vector<std::string> Z_TETROMINO{"** ", " **", "   "};

const std::vector<std::string> L_TETROMINO{"*  ", "***", "   "};

const std::vector<std::string> J_TETROMINO{"  *", "***", "   "};

const std::vector<std::string> O_TETROMINO{"**", "**"};

const std::vector<std::string> I_TETROMINO{"    ", "****", "    ", "    "};

const std::vector<std::vector<std::string>> TETROMINO_SHAPES{
    T_TETROMINO, S_TETROMINO, Z_TETROMINO, L_TETROMINO,
    J_TETROMINO, O_TETROMINO, I_TETROMINO};

} // namespace

TetrominoFactory::TetrominoFactory() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

std::unique_ptr<Tetromino>
TetrominoFactory::createTetromino(const sf::Vector2f &cellSize,
                                  Type tetrominoType) const {
    switch (tetrominoType) {
    case I:
        return std::make_unique<Tetromino>(cellSize, I_TETROMINO);
    case J:
        return std::make_unique<Tetromino>(cellSize, J_TETROMINO);
    case L:
        return std::make_unique<Tetromino>(cellSize, L_TETROMINO);
    case O:
        return std::make_unique<Tetromino>(cellSize, O_TETROMINO);
    case S:
        return std::make_unique<Tetromino>(cellSize, S_TETROMINO);
    case T:
        return std::make_unique<Tetromino>(cellSize, T_TETROMINO);
    case Z:
        return std::make_unique<Tetromino>(cellSize, Z_TETROMINO);
    }
}

std::unique_ptr<Tetromino>
TetrominoFactory::createRandomTetromino(const sf::Vector2f &cellSize) const {
    return createTetromino(cellSize,
                           static_cast<Type>(std::rand() % TETROMINOS_COUNT));
}
