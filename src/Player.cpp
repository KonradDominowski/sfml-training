#include "Player.h"
#include <fmt/base.h>

Player::Player() {
    this->setPosition(100, 100);
    this->setSize(sf::Vector2f(50.f, 50.f));
    this->setFillColor(sf::Color::Cyan);
    this->setOutlineColor(sf::Color::Green);
    this->setOutlineThickness(1.f);
}

/**
 * @brief Moves the player by the specified offset, resolving collisions with game objects.
 *
 * @param offsetX Horizontal movement offset.
 * @param offsetY Vertical movement offset.
 * @param gameObjects Vector of rectangle shapes representing solid objects to check for collisions.
 */
void Player::moveWithCollisionOn(float const offsetX, float const offsetY,
                                 const std::vector<sf::RectangleShape> &gameObjects) {
    sf::RectangleShape::move(offsetX, offsetY);

    auto overlap = sf::FloatRect{};
    const auto playerBounds = this->getGlobalBounds();

    for (auto object: gameObjects) {
        if (object.getGlobalBounds().intersects(playerBounds, overlap)) {
            const auto collisionNormal = object.getPosition() - getPosition();
            const auto manifold = getManifold(overlap, collisionNormal);
            resolve(manifold);
            break;
        }
    }
}

/**
 * @brief Calculates the collision manifold for resolving intersections. \n
 * https://gist.github.com/fallahn/f81d23137409313e7de6
 *
 * @param overlap The overlapping rectangle between the player and an object.
 * @param collisionNormal Vector representing the direction of collision.
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
 * @brief Resolves a collision by moving the player out of the intersecting object.
 *
 * @param manifold A 3D vector containing the collision normal and penetration depth.
 */
void Player::resolve(const sf::Vector3f &manifold) {
    //move the player out of the solid object by the penetration amount
    const auto normal = sf::Vector2f(manifold.x, manifold.y);
    sf::RectangleShape::move(normal * manifold.z);
}
