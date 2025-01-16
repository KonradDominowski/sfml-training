#include "Projectile.h"
#include "Entities/Player.h"
#include <cmath>

#include <fmt/base.h>

#include "Game.h"

/**
 * @brief Constructor of the Projectile class.
 *
 * Initializes a new projectile, setting its position, color, size, and initial angle
 * based on the player's position and the direction of the shot.
 *
 * @param entity Pointer to the Player object, from which the projectile is fired.
 * @param game Pointer to the Game object, which provides the context (e.g., the rendering window).
 */
Projectile::Projectile(Entity *entity, Game *game) {
    this->player_ = entity;
    this->game_ = game;
    this->active = true;
    this->setSize(sf::Vector2f(20.f, 10.f));
    this->setFillColor(sf::Color::Green);
    this->setPosition(sf::Vector2f(entity->getPosition().x + entity->getSize().x / 2,
                                   entity->getPosition().y + entity->getSize().y / 2 - this->getSize().y / 2));
    this->setRotation(calculateInitialAngle());
}

Projectile::~Projectile() {
    // fmt::println("destroy");
}

/**
 * @brief Calculates the initial angle of the projectile based on the player's position and the mouse click location.
 *
 * @return The angle in degrees, defining the direction of the projectile's movement.
 */
float Projectile::calculateInitialAngle() {
    const auto origin = sf::Vector2f(player_->getPosition().x + player_->getSize().x / 2,
                                     player_->getPosition().y + player_->getSize().y / 2);
    const auto clickLocation = sf::Vector2i(sf::Mouse::getPosition(*game_->window).x,
                                            sf::Mouse::getPosition(*game_->window).y);

    const auto destinationInTheWorld = game_->window->mapPixelToCoords(clickLocation);
    const auto direction = destinationInTheWorld - origin;
    const auto angle = std::atan2(direction.y, direction.x);

    this->movementVector = calculateMovementVector(angle);

    const auto angleInDegrees = angle * 180.f / 3.14159f;

    return angleInDegrees;
}

/**
 * @brief Calculates the movement vector of the projectile based on the angle.
 *
 * @param angleInRadians The angle in radians, defining the direction of the projectile's movement.
 * @return A vector representing the movement in units per frame.
 */
sf::Vector2f Projectile::calculateMovementVector(float angleInRadians) const {
    const sf::Vector2f unitDirection(std::cos(angleInRadians), std::sin(angleInRadians));
    const sf::Vector2f movementVector = unitDirection * speed;

    return movementVector;
}

void Projectile::updatePos() {
    move(movementVector);
    checkCollision();
}

/**
 * @brief Checks for collisions between the projectile and other objects in the game.
 *
 * @return true if the projectile collided with another object, false otherwise.
 */
bool Projectile::checkCollision() {
    const auto projectileVertices = this->getVertices(*this); // Funkcja do pobrania wierzchołków
    for (const auto &object: game_->gameObjects) {
        const auto objectVertices = this->getVertices(object); // Analogiczna funkcja dla obiektów

        if (checkRotatedRectCollision(projectileVertices, objectVertices)) {
            this->active = false;
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks for collision between two rotated rectangles using the Separating Axis Theorem (SAT).
 *
 * @param vertices1 The vertices of the first rectangle.
 * @param vertices2 The vertices of the second rectangle.
 * @return true if there is a collision, false otherwise.
 */
bool Projectile::checkRotatedRectCollision(const std::array<sf::Vector2f, 4> &vertices1,
                                           const std::array<sf::Vector2f, 4> &vertices2) {
    // Projection function for the Separating Axis Theorem (SAT)
    auto project = [](const sf::Vector2f &axis, const sf::Vector2f &point) {
        return (point.x * axis.x + point.y * axis.y);
    };

    // Checks if the projections of two rectangles on a given axis overlap
    auto overlapOnAxis = [&](const sf::Vector2f &axis, const std::array<sf::Vector2f, 4> &verticesA,
                             const std::array<sf::Vector2f, 4> &verticesB) {
        float minA = project(axis, verticesA[0]), maxA = minA;
        for (const auto &vertex: verticesA) {
            float projection = project(axis, vertex);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        float minB = project(axis, verticesB[0]), maxB = minB;
        for (const auto &vertex: verticesB) {
            float projection = project(axis, vertex);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        return maxA >= minB && maxB >= minA;
    };

    // Iterate over each edge of both rectangles to check for overlap along the axis perpendicular to each edge
    for (size_t i = 0; i < 4; ++i) {
        const sf::Vector2f edge1 = vertices1[(i + 1) % 4] - vertices1[i];
        const sf::Vector2f edge2 = vertices2[(i + 1) % 4] - vertices2[i];

        sf::Vector2f axis1(-edge1.y, edge1.x);
        sf::Vector2f axis2(-edge2.y, edge2.x);

        if (!overlapOnAxis(axis1, vertices1, vertices2) ||
            !overlapOnAxis(axis2, vertices1, vertices2)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Returns the vertices of the given rectangle shape in global coordinates.
 *
 * @param object The rectangle shape for which the vertices are calculated.
 * @return An array containing the four vertices of the rectangle in global coordinates.
 */
std::array<sf::Vector2f, 4> Projectile::getVertices(const sf::RectangleShape &object) {
    const sf::Transform transform = object.getTransform();
    const sf::Vector2f topLeft = transform.transformPoint(0.f, 0.f);
    const sf::Vector2f topRight = transform.transformPoint(object.getSize().x, 0.f);
    const sf::Vector2f bottomLeft = transform.transformPoint(0.f, object.getSize().y);
    const sf::Vector2f bottomRight = transform.transformPoint(object.getSize().x, object.getSize().y);

    return {topLeft, topRight, bottomRight, bottomLeft};
}
