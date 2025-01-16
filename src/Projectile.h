#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

struct Player;
struct Game;

struct Projectile : sf::RectangleShape {
public:
    // Constructor
    explicit Projectile(Player *player, Game *game);

    ~Projectile() override;

    // Variables
    Player *player_;
    Game *game_;
    bool active;
    float speed = 30.f;
    sf::Vector2f movementVector = {1.f, 1.f};

    // Methods
    float calculateInitialAngle();

    void setHitbox();

    sf::Vector2f calculateMovementVector(float angleInRadians) const;

    void updatePos();

    bool checkCollision();
    bool checkRotatedRectCollision(const std::array<sf::Vector2f, 4> &vertices1,
                                   const std::array<sf::Vector2f, 4> &vertices2);
    std::array<sf::Vector2f, 4> getVertices(const sf::RectangleShape &object);

};


#endif //PROJECTILE_H
