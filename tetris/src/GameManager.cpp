#include "GameManager.hpp"

#include "EventManager.hpp"
#include "Field.hpp"
#include "ScoreBoard.hpp"
#include "Tetromino.hpp"
#include "TetrominoFactory.hpp"

#include <stdexcept>

GameManager::GameManager(std::unique_ptr<Field> field,
                         std::unique_ptr<ScoreBoard> scoreBoard,
                         std::unique_ptr<TetrominoFactory> tetrominoFactory,
                         const std::shared_ptr<EventManager> &eventManager)
    : m_field{std::move(field)}, m_scoreBoard{std::move(scoreBoard)},
      m_tetrominoFactory{std::move(tetrominoFactory)},
      m_eventManager{eventManager} {
    if (!m_field) {
        throw std::invalid_argument("Field is not initialized");
    }

    if (!m_scoreBoard) {
        throw std::invalid_argument("ScoreBoard is not initialized");
    }

    if (!m_tetrominoFactory) {
        throw std::invalid_argument("TetrominoFactory is not initialized");
    }

    if (!m_eventManager) {
        throw std::invalid_argument("EventManager is not initialized");
    }

    m_eventManager->subscribe("TetrominoPlaced", [&](const std::any &) {
        m_field->addTetromino(
            m_tetrominoFactory->createRandomTetromino(m_field->getCellSize()));
    });

    m_eventManager->subscribe(
        "FieldIsFull", [&](const std::any &) { m_gameIsRunning = false; });
}

void GameManager::update(float deltaTime) {
    if (m_gameIsRunning) {
        m_timeSinceLastTetrominoMoveDown += deltaTime;

        if (m_timeSinceLastTetrominoMoveDown > 1.0f) {
            m_field->moveTetrominoDown();
            m_timeSinceLastTetrominoMoveDown = 0.0f;
        }
    }
}

void GameManager::draw(sf::RenderTarget &target) const {
    m_field->draw(target);
    m_scoreBoard->draw(target);
}

bool GameManager::isGameRunning() const { return m_gameIsRunning; }

void GameManager::startGame() {
    m_field->clear();
    m_field->addTetromino(
        m_tetrominoFactory->createRandomTetromino(m_field->getCellSize()));
    m_gameIsRunning = true;
}

void GameManager::moveTetrominoLeft() { m_field->moveTetrominoLeft(); }

void GameManager::moveTetrominoRight() { m_field->moveTetrominoRight(); }

void GameManager::moveTetrominoDown() { m_field->moveTetrominoDown(); }

void GameManager::dropTetromino() { m_field->dropTetromino(); }

void GameManager::rotateTetrominoClockwise() {
    m_field->rotateTetrominoClockwise();
}

void GameManager::rotateTetrominoCounterClockwise() {
    m_field->rotateTetrominoCounterClockwise();
}
