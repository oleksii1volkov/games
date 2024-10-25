#include "ScoreBoard.hpp"

#include "EventManager.hpp"

#include <stdexcept>

ScoreBoard::ScoreBoard(const sf::Vector2f &position, const sf::Vector2f &size,
                       const std::shared_ptr<EventManager> &eventManager)
    : m_position{position}, m_size{size}, m_eventManager{eventManager} {
    if (!m_eventManager) {
        throw std::invalid_argument("EventManager is not initialized");
    }

    buildUI();
    subscribeToEvents();
}

void ScoreBoard::buildUI() {
    m_boldFont.loadFromFile("resources/fonts/SourceCodePro-Bold.ttf");
    m_regularFont.loadFromFile("resources/fonts/SourceCodePro-Regular.ttf");

    m_background.setSize(m_size);
    m_background.setFillColor(sf::Color{244, 250, 255});
    m_background.setOutlineColor(sf::Color{217, 217, 213});
    m_background.setOutlineThickness(2);
    m_background.setPosition(m_position);

    m_scoreLabelText.setString("Score:");
    m_scoreLabelText.setFont(m_regularFont);
    m_scoreLabelText.setCharacterSize(24);
    m_scoreLabelText.setFillColor(sf::Color::Magenta);
    m_scoreLabelText.setPosition(m_position + sf::Vector2f{10, 10});

    m_scoreText.setString(std::to_string(m_score));
    m_scoreText.setFont(m_boldFont);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::Cyan);
    m_scoreText.setPosition(m_position + sf::Vector2f{10, 40});

    m_levelLabelText.setString("Level:");
    m_levelLabelText.setFont(m_regularFont);
    m_levelLabelText.setCharacterSize(24);
    m_levelLabelText.setFillColor(sf::Color::Magenta);
    m_levelLabelText.setPosition(m_position + sf::Vector2f{10, 70});

    m_levelText.setString(std::to_string(m_level));
    m_levelText.setFont(m_boldFont);
    m_levelText.setCharacterSize(24);
    m_levelText.setFillColor(sf::Color::Cyan);
    m_levelText.setPosition(m_position + sf::Vector2f{10, 100});

    m_linesLabelText.setString("Lines:");
    m_linesLabelText.setFont(m_regularFont);
    m_linesLabelText.setCharacterSize(24);
    m_linesLabelText.setFillColor(sf::Color::Magenta);
    m_linesLabelText.setPosition(m_position + sf::Vector2f{10, 130});

    m_linesText.setString(std::to_string(m_linesCount));
    m_linesText.setFont(m_boldFont);
    m_linesText.setCharacterSize(24);
    m_linesText.setFillColor(sf::Color::Cyan);
    m_linesText.setPosition(m_position + sf::Vector2f{10, 160});
}

void ScoreBoard::subscribeToEvents() {
    m_eventManager->subscribe("RowsEliminated", [&](const std::any &payload) {
        try {
            const auto rowsCount{std::any_cast<size_t>(payload)};
            onRowsEliminated(rowsCount);
        } catch (const std::bad_any_cast &exception) {
        }
    });

    m_eventManager->subscribe(
        "TetrominoForcedMovedDown",
        [&](const std::any &payload) { onTetrominoForcedMovedDown(); });

    m_eventManager->subscribe("TetrominoDropped", [&](const std::any &payload) {
        try {
            const auto rowsCount{std::any_cast<size_t>(payload)};
            onTetrominoDropped(rowsCount);
        } catch (const std::bad_any_cast &exception) {
        }
    });
}

void ScoreBoard::increaseScore(size_t amount) {
    m_score += amount;
    m_scoreText.setString(std::to_string(m_score));
}

void ScoreBoard::draw(sf::RenderTarget &target) const {
    target.draw(m_background);
    target.draw(m_scoreLabelText);
    target.draw(m_scoreText);
    target.draw(m_levelLabelText);
    target.draw(m_levelText);
    target.draw(m_linesLabelText);
    target.draw(m_linesText);
}

void ScoreBoard::onRowsEliminated(size_t rowsCount) {
    increaseScore(rowsCount * 100);
    m_linesCount += rowsCount;
}

void ScoreBoard::onTetrominoForcedMovedDown() { increaseScore(1); }

void ScoreBoard::onTetrominoDropped(size_t rowsCount) {
    increaseScore(rowsCount * 2);
}
