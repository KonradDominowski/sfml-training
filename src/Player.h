//
// Created by konrad on 06.01.25.
//
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>


struct Player : sf::RectangleShape {
private:
public:

    // Variables
    float speed = 5.f;
    float acceleration = 1.f;

    // Constructor
    Player();

    void moveWithCollisionOn(float offsetX, float  offsetY, const std::vector<sf::RectangleShape> &gameObjects);
    sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);
    void resolve(const sf::Vector3f& manifold);

};



#endif //PLAYER_H
