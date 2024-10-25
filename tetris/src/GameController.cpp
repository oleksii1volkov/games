#include "GameController.hpp"

#include "EventManager.hpp"
#include "GameManager.hpp"

#include <SFML/Window/Event.hpp>

GameController::GameController(
    const std::shared_ptr<GameManager> &gameManager,
    const std::shared_ptr<EventManager> &eventManager)
    : m_gameManager{gameManager}, m_eventManager{eventManager} {
    if (!m_gameManager) {
        throw std::invalid_argument("GameManager is not initialized");
    }

    if (!m_eventManager) {
        throw std::invalid_argument("EventManager is not initialized");
    }
}

void GameController::handleInput(sf::RenderWindow &window) {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (m_gameManager->isGameRunning()) {
                if (event.key.code == sf::Keyboard::Left) {
                    m_gameManager->moveTetrominoLeft();
                } else if (event.key.code == sf::Keyboard::Up) {
                    m_gameManager->rotateTetrominoClockwise();
                } else if (event.key.code == sf::Keyboard::Right) {
                    m_gameManager->moveTetrominoRight();
                } else if (event.key.code == sf::Keyboard::Down) {
                    m_eventManager->publish("TetrominoForcedMovedDown",
                                            std::nullopt);
                    m_gameManager->moveTetrominoDown();
                } else if (event.key.code == sf::Keyboard::Z) {
                    m_gameManager->rotateTetrominoCounterClockwise();
                } else if (event.key.code == sf::Keyboard::Space) {
                    m_gameManager->dropTetromino();
                }
            } else {
                if (event.key.code == sf::Keyboard::Space) {
                    m_gameManager->startGame();
                }
            }
        }
    }
}
