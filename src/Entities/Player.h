#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "../Projectile.h"

struct Game;
struct Projectile;

struct Player : Entity {
public:
    // Variables
    float speed = 300.f;
    float acceleration = 1.f;
    std::vector<Projectile> projectiles;

    // Jumping variables
    sf::Vector2f velocity;
    bool canJump;
    float jumpHeight = 700.f;

    // Game
    Game *game_;


    // Constructor
    explicit Player(Game *game);


    // Functions
    void update(float deltaTime);

    void moveWithCollisionOn(sf::Vector2f &velocity, float deltaTime);
    // void moveWithCollisionOn(float offsetX, float offsetY, const std::vector<sf::RectangleShape> &gameObjects);

    sf::Vector3f getManifold(const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal);

    bool checkLanding(const sf::RectangleShape &object);

    void resolve(const sf::Vector3f &manifold);

    void shoot();

    void addProjectile(const Projectile &projectile);
};


#endif //PLAYER_H
