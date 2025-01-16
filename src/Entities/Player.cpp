#include "Player.h"

#include <cmath>
#include <fmt/base.h>

#include "../Game.h"
#include "../Projectile.h"

Player::Player(Game *game) {
    this->setPosition(100, 400);
    this->setSize(sf::Vector2f(100.f, 100.f));
    this->setFillColor(sf::Color::Cyan);
    this->game_ = game;
}

/**
 * @brief Updates the player's state, including movement and collision handling.
 *
 * @param deltaTime The time elapsed since the last update, in seconds.
 * @param gameObjects A vector of rectangle shapes representing objects in the game world.
 */
void Player::update(float deltaTime) {
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

    auto toDelete = std::vector<std::vector<Projectile>::iterator>();

    for (auto it = projectiles.begin(); it != projectiles.end(); it++) {
        it->updatePos();
        if (not it->active) toDelete.push_back(it);
    }

    for (auto it: toDelete) {
        projectiles.erase(it);
    }
}

/**
 * @brief Moves the player by the specified offset, resolving collisions with game objects.
 *
 * @param velocity Movement offset
 * @param gameObjects A vector of rectangle shapes representing solid objects to check for collisions.
 */
void Player::moveWithCollisionOn(sf::Vector2f &velocity, float deltaTime) {
    auto [offsetX , offsetY] = velocity * deltaTime;
    auto &gameObjects = game_->gameObjects;
    std::vector<std::pair<sf::RectangleShape, sf::Vector3f> > objectsManifolds;

    sf::RectangleShape::move(offsetX, offsetY);

    auto overlap = sf::FloatRect{};
    const auto playerBounds = this->getGlobalBounds();

    for (const auto &object: gameObjects) {
        if (object.getGlobalBounds().intersects(playerBounds, overlap)) {
            const auto collisionNormal = object.getPosition() - getPosition();
            const auto manifold = getManifold(overlap, collisionNormal);

            // Stop ascending when player head hits something
            if (manifold.y > 0) velocity.y *= 0.3f;
            objectsManifolds.emplace_back(object, manifold);
        }
    }

    for (const auto &[object, manifold]: objectsManifolds) {
        canJump = checkLanding(object);
        if (canJump) break;
    }

    for (const auto &[object, manifold]: objectsManifolds) {
        resolve(manifold);
    }
}

/**
 * @brief Calculates the collision manifold for resolving intersections.
 *
 * The manifold contains information needed to resolve the collision,
 * including direction and depth of penetration.
 *
 * @param overlap The overlapping rectangle between the player and an object.
 * @param collisionNormal A vector representing the direction of collision.
 *
 * @return A 3D vector where:
 * - x and y represent the collision normal (direction of resolution).
 * - z represents the penetration depth (amount to move to resolve the collision).
 */
sf::Vector3f Player::getManifold(const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal) {
    sf::Vector3f manifold;
    if (overlap.width < overlap.height) {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    } else {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}

/**
 * @brief Checks if the player has landed on an object.
 *
 * @param object A rectangle shape representing the object to check against.
 * @return True if the player has landed, false otherwise.
 */
bool Player::checkLanding(const sf::RectangleShape &object) {
    auto landed = false;

    const float playerBottom = this->getGlobalBounds().top + this->getGlobalBounds().height;
    float objectTop = object.getGlobalBounds().top;

    // Te liczby dają margines przy sprawdzaniu, czy prędkość pionowa osiągnęła 0
    if (playerBottom >= objectTop - 40.f && playerBottom <= objectTop + 40.f && velocity.y > 0) {
        landed = true;
        velocity.y = 0.f;
    }

    return landed;
}

/**
 * @brief Resolves a collision by moving the player out of the intersecting object.
 *
 * @param manifold A 3D vector containing the collision normal and penetration depth.
 */
void Player::resolve(const sf::Vector3f &manifold) {
    //move the player out of the solid object by the penetration amount
    const auto normal = sf::Vector2f(manifold.x, manifold.y);
    sf::RectangleShape::move(normal * manifold.z);
}

void Player::shoot() {
    const auto projectile = Projectile(this, game_);
    addProjectile(projectile);
}

void Player::addProjectile(const Projectile &projectile) {
    this->projectiles.push_back(projectile);
}
