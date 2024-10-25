#include "EventManager.hpp"
#include "Field.hpp"
#include "GameController.hpp"
#include "GameManager.hpp"
#include "ScoreBoard.hpp"
#include "Tetromino.hpp"
#include "TetrominoFactory.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

namespace {
std::shared_ptr<EventManager> createEventManager() {
    return std::make_shared<EventManager>();
}

std::unique_ptr<Field>
createField(const std::shared_ptr<EventManager> &eventManager) {
    const sf::Vector2f cellSize{40, 40};
    const sf::Vector2u gridSize{10, 20};

    return std::make_unique<Field>(sf::Vector2f{0, 0}, cellSize, gridSize,
                                   eventManager);
}

std::unique_ptr<ScoreBoard>
createScoreBoard(const std::shared_ptr<EventManager> &eventManager) {
    return std::make_unique<ScoreBoard>(sf::Vector2f{400, 0},
                                        sf::Vector2f{400, 800}, eventManager);
}

std::shared_ptr<GameManager>
createGameManager(std::unique_ptr<Field> field,
                  std::unique_ptr<ScoreBoard> scoreBoard,
                  std::unique_ptr<TetrominoFactory> tetrominoFactory,
                  const std::shared_ptr<EventManager> &eventManager) {
    return std::make_shared<GameManager>(
        std::move(field), std::move(scoreBoard), std::move(tetrominoFactory),
        eventManager);
}

std::unique_ptr<GameController>
createGameController(const std::shared_ptr<GameManager> &gameManager,
                     const std::shared_ptr<EventManager> &eventManager) {
    return std::make_unique<GameController>(gameManager, eventManager);
}

std::unique_ptr<TetrominoFactory> createTetrominoFactory() {
    return std::make_unique<TetrominoFactory>();
}

} // namespace

int main(int argc, char *argv[]) {
    const auto eventManager{createEventManager()};
    auto field{createField(eventManager)};
    auto scoreBoard{createScoreBoard(eventManager)};
    auto tetrominoFactory{createTetrominoFactory()};

    const auto gameManager{
        createGameManager(std::move(field), std::move(scoreBoard),
                          std::move(tetrominoFactory), eventManager)};
    const auto gameController{createGameController(gameManager, eventManager)};

    const auto windowSize{sf::Vector2u{800, 800}};
    sf::RenderWindow window{sf::VideoMode{windowSize.x, windowSize.y},
                            "Tetris"};

    sf::Clock clock;

    while (window.isOpen()) {
        const float deltaTime{clock.restart().asSeconds()};

        gameController->handleInput(window);
        gameManager->update(deltaTime);

        window.clear();
        gameManager->draw(window);
        window.display();
    }

    return 0;
}
