#include "Entity.h"
#include "../Game.h"

Entity::Entity(Game *game) {
    this->game_ = game;
}

Entity::Entity(Game *game, const float speed, const float acceleration) {
    this->game_ = game;
    this->speed = speed;
    this->acceleration = acceleration;
}

void Entity::update(float deltaTime) {
}

void Entity::moveWithCollisionOn(sf::Vector2f &velocity, float deltaTime) {
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

sf::Vector3f Entity::getManifold(const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal) const {
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

bool Entity::checkLanding(const sf::RectangleShape &object) {
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

void Entity::resolve(const sf::Vector3f &manifold) {
    //move the player out of the solid object by the penetration amount
    const auto normal = sf::Vector2f(manifold.x, manifold.y);
    sf::RectangleShape::move(normal * manifold.z);
}

void Entity::shoot() {
    const auto projectile = Projectile(this, game_);
    addProjectile(projectile);
}

void Entity::addProjectile(const Projectile &projectile) {
    this->projectiles.push_back(projectile);
}

void Entity::updateProjectiles() {
    auto toDelete = std::vector<std::vector<Projectile>::iterator>();

    for (auto it = this->projectiles.begin(); it != this->projectiles.end(); it++) {
        it->updatePos();
        if (not it->active) toDelete.push_back(it);
    }

    for (auto it: toDelete) {
        this->projectiles.erase(it);
    }
}
