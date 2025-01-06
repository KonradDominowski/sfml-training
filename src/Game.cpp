#include "Game.h"

// Private functions
void Game::initializeVariables() {
    this->window = nullptr;
}

void Game::initWindow() {
    videoMode.width = 1280;
    videoMode.height = 720;

    window = new sf::RenderWindow(this->videoMode, "First Game", sf::Style::Close);
    window->setFramerateLimit(60);
}

void Game::initEnemies() {
    enemy.setPosition(100, 100);
    enemy.setSize(sf::Vector2f(100.f, 100.f));
    enemy.setFillColor(sf::Color::Cyan);
    enemy.setOutlineColor(sf::Color::Green);
    enemy.setOutlineThickness(1.f);
}

void Game::initBox() {
    ground.setPosition(0, 600);
    ground.setSize(sf::Vector2f(1280.f, 200.f));
    ground.setFillColor(sf::Color::Green);
    ground.setOutlineColor(sf::Color::Black);
    ground.setOutlineThickness(1.f);
    gameObjects.push_back(ground);
}

void Game::initPlayer() {
    player = Player();
}

// Constructors / Destructors
Game::Game() {
    initBox();
    initPlayer();
    initializeVariables();
    initWindow();
    initEnemies();
}

Game::~Game() {
    delete this->window;
}

bool Game::running() const {
    return this->window->isOpen();
}

void Game::pollEvents(float deltaTime) {
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        window->close();
                        break;
                }
            default: ;
        }
    }

    player.update(deltaTime, gameObjects);
}

void Game::updateMousePos() {
    mousePosWindow = sf::Mouse::getPosition(*window);
}

// Functions
void Game::update(float deltaTime) {
    updateMousePos();
    pollEvents(deltaTime);
}

void Game::render() {
    window->clear(sf::Color::White);

    window->draw(ground);
    window->draw(player);

    auto box = sf::RectangleShape(sf::Vector2f(50, 50));
    box.setFillColor(sf::Color::Yellow);
    box.setOutlineColor(sf::Color::Black);
    box.setPosition(400, 300);
    box.setOutlineThickness(1.f);
    gameObjects.push_back(box);

    window->draw(box);

    window->display();
}
