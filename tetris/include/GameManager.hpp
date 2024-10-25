#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

class EventManager;
class Field;
class ScoreBoard;
class TetrominoFactory;

class GameManager {
public:
    GameManager(std::unique_ptr<Field> field,
                std::unique_ptr<ScoreBoard> scoreBoard,
                std::unique_ptr<TetrominoFactory> tetrominoFactory,
                const std::shared_ptr<EventManager> &eventManager);

    void update(float deltaTime);
    void draw(sf::RenderTarget &target) const;

    bool isGameRunning() const;
    void startGame();
    void pauseGame(bool paused);

    void moveTetrominoLeft();
    void moveTetrominoRight();
    void moveTetrominoDown();
    void dropTetromino();
    void rotateTetrominoClockwise();
    void rotateTetrominoCounterClockwise();

private:
    std::unique_ptr<Field> m_field;
    std::unique_ptr<ScoreBoard> m_scoreBoard;
    std::unique_ptr<TetrominoFactory> m_tetrominoFactory;
    std::shared_ptr<EventManager> m_eventManager;

    bool m_gameIsRunning{false};
    float m_timeSinceLastTetrominoMoveDown{0.0f};
};
