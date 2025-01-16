#include "Player.h"

#include <cmath>
#include <fmt/base.h>

#include "../Game.h"

Player::Player(Game *game): Entity(game_) {
    this->setPosition(100, 400);
    this->setSize(sf::Vector2f(100.f, 100.f));
    this->setFillColor(sf::Color::Cyan);
    this->game_ = game;
}

void Player::update(const float deltaTime) {
    /* Ruch wewnątrz pollEvent: wykonywany tylko, gdy wystąpi zdarzenie (np. naciśnięcie klawisza),
   co może prowadzić do mniej płynnego działania. Sprawdza jednorazowe zdarzenia, a nie stan klawisza.

   Ruch poza pollEvent: sprawdzany w każdej iteracji pętli gry, dzięki czemu działa płynnie podczas
   trzymania klawisza, idealny do ruchu w czasie rzeczywistym. */

    // Simulates friction for horizontal movement. The lower the value the higher the friction.
    velocity.x *= 0.6f;

    // Adjusts vertical velocity for realistic jumping physics.
    if (velocity.y < 0) { velocity.y *= 0.94f; } else { velocity.y *= 1.f; }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        this->setPosition(100, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) and canJump) {
        canJump = false;
        velocity.y = -sqrtf(2.f * game_->gravity * jumpHeight);
    }

    velocity.y += game_->gravity * deltaTime;

    moveWithCollisionOn(velocity, deltaTime);
    updateProjectiles();
}