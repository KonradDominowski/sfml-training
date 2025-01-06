#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <fmt/base.h>
#include <vector>

#include "Player.h"

/*
 * Class that acts as the game engine. Wrapper class/
 */

class Game {
private:
    // Variables
    float gravity = 5.f;

    // Window
    sf::RenderWindow *window;
    sf::VideoMode videoMode;

    // Mouse positions
    sf::Vector2i mousePosWindow;

    // Game objects
    Player player;
    sf::RectangleShape enemy;
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape ground;
    std::vector<sf::RectangleShape> gameObjects;


    // Private functions
    void initializeVariables();
    void initWindow();
    void initEnemies();
    void initBox();
    void initPlayer();


public:
    // Constructors / Destructors
    sf::Event event;

    Game();
    virtual ~Game();

    // Accessors
    bool running() const;

    // Functions
    void update(float deltaTime);
    void pollEvents(float deltaTime);
    void updateMousePos();
    void render() ;
};


#endif //GAME_H
