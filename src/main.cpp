#include "Game.h"

auto main() -> int {
    // Window
    auto game = Game();

    // Game loop
    while (game.running()) {
        game.update();
        game.render();
    }
}
