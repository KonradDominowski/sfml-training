#include "Game.h"

auto main() -> int {
    // Window
    auto game = Game();

    auto deltaTime = 0.f;
    auto clock = sf::Clock();

    // Game loop
    while (game.running()) {
        deltaTime = clock.restart().asSeconds();

        

        game.update(deltaTime);
        game.render();
    }
}
