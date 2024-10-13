#include "ScoreCounter.hpp"

namespace {
size_t getDigitsCount(size_t number) {
    if (number == 0) {
        return 1;
    }

    return std::floor(std::log10(number)) + 1;
}
} // namespace

ScoreCounter::ScoreCounter(const AssetManager &assetManager)
    : m_assetManager{assetManager} {
    m_digitSprites.emplace_back(assetManager.getTexture("0"));
}

void ScoreCounter::draw(sf::RenderTarget &target) {
    float totalWidth{0.0f};

    for (const auto &digitSprite : m_digitSprites) {
        totalWidth += digitSprite.getLocalBounds().width;
    }

    float xPosition{m_position.x};

    for (auto &digitSprite : m_digitSprites) {
        digitSprite.setPosition(xPosition - totalWidth / 2.0f, m_position.y);
        xPosition += digitSprite.getLocalBounds().width;

        target.draw(digitSprite);
    }
}

void ScoreCounter::setPosition(const sf::Vector2f &position) {
    m_position = position;
}

void ScoreCounter::increment() {
    ++m_counter;

    const auto digitsCount{getDigitsCount(m_counter)};

    if (digitsCount > m_digitSprites.size()) {
        m_digitSprites.emplace_back(m_assetManager.getTexture("0"));
    }

    size_t digitIndex{0};
    auto counter{m_counter};

    while (counter > 0) {
        auto &digitSprite{
            m_digitSprites[m_digitSprites.size() - digitIndex - 1]};

        digitSprite.setTexture(
            m_assetManager.getTexture(std::to_string(counter % 10)), true);

        ++digitIndex;
        counter /= 10;
    }
}

void ScoreCounter::reset() {
    m_counter = 0;
    m_digitSprites.clear();
    m_digitSprites.emplace_back(m_assetManager.getTexture("0"));
}
