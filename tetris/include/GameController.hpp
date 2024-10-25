#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class EventManager;
class GameManager;

class GameController {
public:
    GameController(const std::shared_ptr<GameManager> &gameManager,
                   const std::shared_ptr<EventManager> &eventManager);

    void handleInput(sf::RenderWindow &window);

private:
    std::shared_ptr<GameManager> m_gameManager;
    std::shared_ptr<EventManager> m_eventManager;
};
