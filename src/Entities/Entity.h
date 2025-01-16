#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics.hpp"

struct Game;
struct Projectile;

struct Entity : sf::RectangleShape {
    // Variables
    float speed = 300.f;
    float acceleration = 1.f;
    std::vector<Projectile> projectiles;

    // Game
    Game *game_;

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



#endif //ENTITY_H
