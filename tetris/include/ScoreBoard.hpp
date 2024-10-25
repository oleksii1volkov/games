#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>

class EventManager;

class ScoreBoard {
public:
    ScoreBoard(const sf::Vector2f &position, const sf::Vector2f &size,
               const std::shared_ptr<EventManager> &eventManager);

    void draw(sf::RenderTarget &target) const;

private:
    void buildUI();
    void subscribeToEvents();
    void increaseScore(size_t amount);

    void onRowsEliminated(size_t rowsCount);
    void onTetrominoForcedMovedDown();
    void onTetrominoDropped(size_t rowsCount);

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Font m_boldFont;
    sf::Font m_regularFont;
    std::shared_ptr<EventManager> m_eventManager;
    size_t m_score{0};
    size_t m_level{0};
    size_t m_linesCount{0};
    sf::RectangleShape m_background;
    sf::Text m_scoreText;
    sf::Text m_scoreLabelText;
    sf::Text m_levelText;
    sf::Text m_levelLabelText;
    sf::Text m_linesText;
    sf::Text m_linesLabelText;
};
