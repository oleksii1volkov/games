#include "GameManager.hpp"

#include "AssetManager.hpp"
#include "Bird.hpp"
#include "Config.hpp"
#include "Pipe.hpp"
#include "ScoreCounter.hpp"

#include <cstdlib>
#include <utility>
#include <vector>

#include <SFML/Window/Event.hpp>

GameManager::GameManager(const AssetManager &assetManager)
    : m_assetManager{assetManager} {
    std::srand(std::time(nullptr));
}

namespace {
std::pair<Pipe, Pipe> createRandomPipes(const float xPosition,
                                        const sf::Vector2u &windowSize,
                                        const sf::Vector2f &scaleFactor,
                                        const AssetManager &assetManager) {
    const unsigned int minPipeHeight{100};
    const float gapYPosition(std::rand() %
                                 (windowSize.y - minPipeHeight * 2 - PIPE_GAP) +
                             minPipeHeight);

    Pipe upperPipe{assetManager};
    upperPipe.setScale(scaleFactor);
    upperPipe.setOrigin(
        {upperPipe.getBounds().width / scaleFactor.x, upperPipe.getOrigin().y});
    upperPipe.setRotation(180);
    upperPipe.setPosition(sf::Vector2f{xPosition, gapYPosition});

    Pipe lowerPipe{assetManager};
    lowerPipe.setScale(scaleFactor);
    lowerPipe.setPosition(sf::Vector2f{xPosition, gapYPosition + PIPE_GAP});

    return std::make_pair(std::move(upperPipe), std::move(lowerPipe));
}

bool isOffScreen(const Pipe &pipe) {
    return pipe.getPosition().x < -pipe.getBounds().width;
}

bool isColliding(const Bird &bird, const Pipe &pipe) {
    return bird.getBounds().intersects(pipe.getBounds());
}
} // namespace

void GameManager::run(sf::RenderWindow &window) {
    const auto windowSize{window.getSize()};
    const auto &backgroundTexture{m_assetManager.getTexture("background")};

    const sf::Vector2f scaleFactor{
        static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.y) / backgroundTexture.getSize().y};

    sf::Sprite backgroundSprite{backgroundTexture};
    backgroundSprite.setScale(scaleFactor);

    ScoreCounter scoreCounter{m_assetManager};
    scoreCounter.setPosition({windowSize.x / 2.0f, 100.0f});

    const sf::Vector2f birdStartPosition{windowSize.x / 2.0f,
                                         windowSize.y / 2.0f};
    Bird bird{m_assetManager};
    bird.setScale(scaleFactor);
    bird.setPosition(birdStartPosition);

    std::vector<std::pair<Pipe, Pipe>> pipes;

    sf::Clock pipesClock;
    sf::Clock gameClock;
    bool gameIsRunning{true};

    size_t lastCreatedPipeId{0};
    size_t lastPassedPipeId{0};

    auto restartGame = [&]() {
        scoreCounter.reset();
        bird.setPosition(birdStartPosition);
        bird.setVelocity(0);
        pipes.clear();
        pipesClock.restart();
        gameIsRunning = true;
    };

    while (window.isOpen()) {
        sf::Time deltaTime{gameClock.restart()};
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (gameIsRunning) {
                        bird.jump();
                    } else {
                        restartGame();
                    }
                }
            }
        }

        if (gameIsRunning) {
            bird.update(deltaTime.asSeconds());

            if (bird.getPosition().y > windowSize.y) {
                gameIsRunning = false;
            }

            if (pipesClock.getElapsedTime().asSeconds() > PIPE_SPAWN_INTERVAL) {
                pipes.emplace_back(createRandomPipes(
                    windowSize.x, windowSize, scaleFactor, m_assetManager));
                pipesClock.restart();
                ++lastCreatedPipeId;
            }

            if (!pipes.empty()) {
                const auto &[frontUpperPipe, frontLowerPipe] = pipes.front();

                if (isOffScreen(frontUpperPipe)) {
                    pipes.erase(pipes.begin());
                }

                for (size_t pipeIndex{0}; pipeIndex < pipes.size();
                     ++pipeIndex) {
                    auto &[upperPipe, lowerPipe] = pipes.at(pipeIndex);
                    const auto pipeId{lastCreatedPipeId - pipes.size() +
                                      pipeIndex + 1};

                    if (bird.getPosition().x >
                            upperPipe.getPosition().x +
                                upperPipe.getBounds().width &&
                        pipeId != lastPassedPipeId) {
                        lastPassedPipeId = pipeId;
                        scoreCounter.increment();
                    }

                    if (isColliding(bird, upperPipe) ||
                        isColliding(bird, lowerPipe)) {
                        gameIsRunning = false;
                    } else {
                        upperPipe.update(deltaTime.asSeconds());
                        lowerPipe.update(deltaTime.asSeconds());
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        for (auto &[upperPipe, lowerPipe] : pipes) {
            upperPipe.draw(window);
            lowerPipe.draw(window);
        }

        bird.draw(window);
        scoreCounter.draw(window);

        window.display();
    }
}
