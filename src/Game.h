#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <fmt/base.h>
#include <vector>

#include "Entities/Player.h"

/*
 * Class that acts as the game engine. Wrapper class/
 */

struct Game {
private:
    // Variables

    // Window
    sf::VideoMode videoMode;

    // View
    sf::View view;

    // Mouse positions
    sf::Vector2i mousePosWindow;

    // Game objects
    Player player;
    sf::RectangleShape enemy;
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape ground;
    sf::RectangleShape wall;

public:


private:
    // Private functions
    void initializeVariables();

    void initWindow();

    void initGameObjects();

    void initGround();

    void initBox(float x, float y);

    void initEnemies();

public:
    // Variables
    sf::Event event;
    sf::RenderWindow *window;
    std::vector<sf::RectangleShape> gameObjects;
    float gravity = 4000.f;


    // Constructors / Destructors

    Game();

    virtual ~Game();


    // Accessors
    bool running() const;

    // Functions
    sf::WindowBase *getWindow() const;

    void update(float deltaTime);

    void pollEvents(float deltaTime);

    void updateMousePos();

    void render();
};


#endif //GAME_H
