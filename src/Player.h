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
    float speed = 300.f;
    float acceleration = 1.f;

    // Jumping variables
    sf::Vector2f velocity;
    bool canJump;
    float jumpHeight = 350.f;

    // Constructor
    Player();

    void update(float deltaTime, const std::vector<sf::RectangleShape> &gameObjects);
    void moveWithCollisionOn(float offsetX, float offsetY, const std::vector<sf::RectangleShape> &gameObjects);
    sf::Vector3f getManifold(const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal);
    bool checkLanding(const sf::RectangleShape &object);
    void resolve(const sf::Vector3f &manifold);
};


#endif //PLAYER_H
