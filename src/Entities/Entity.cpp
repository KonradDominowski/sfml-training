#include "Entity.h"

void Entity::update(float deltaTime) {
}

void Entity::moveWithCollisionOn(sf::Vector2f &velocity, float deltaTime) {
}

sf::Vector3f Entity::getManifold(const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal) {
}

bool Entity::checkLanding(const sf::RectangleShape &object) {
}

void Entity::resolve(const sf::Vector3f &manifold) {
}

void Entity::shoot() {
}

void Entity::addProjectile(const Projectile &projectile) {
}
