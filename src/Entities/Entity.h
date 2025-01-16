#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics/RectangleShape.hpp"

#include "../Projectile.h"

struct Game;

struct Entity : sf::RectangleShape {
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

    explicit Entity(Game *game);

    Entity(Game *game, float speed, float acceleration);

    virtual void update(float deltaTime);

    void moveWithCollisionOn(sf::Vector2f &velocity, float deltaTime);

    sf::Vector3f getManifold(const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal) const;

    bool checkLanding(const sf::RectangleShape &object);

    void resolve(const sf::Vector3f &manifold);

    void shoot();

    void addProjectile(const Projectile &projectile);

    void updateProjectiles();

};

#endif //ENTITY_H
