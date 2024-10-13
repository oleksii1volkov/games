#include "AssetManager.hpp"
#include "Config.hpp"
#include "GameManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

int main() {
    const AssetManager assetManager;
    GameManager gameManager{assetManager};

    sf::RenderWindow window(sf::VideoMode{WINDOW_WIDTH, WINDOW_HEIGHT},
                            "Flappy Bird");
    window.setVerticalSyncEnabled(true);

    gameManager.run(window);

    return 0;
}
