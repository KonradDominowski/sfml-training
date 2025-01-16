#include "Game.h"


// Constructors / Destructors
Game::Game(): player(this) {
    initGameObjects();
    initializeVariables();
    initWindow();
    initEnemies();
}


// Private functions
void Game::initializeVariables() {
    this->window = nullptr;
}

void Game::initWindow() {
    videoMode.width = 1280;
    videoMode.height = 720;

    window = new sf::RenderWindow(this->videoMode, "First Game", sf::Style::Close);
    window->setFramerateLimit(60);
    view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(1280.f, 720.f));
}

void Game::initGameObjects() {
    initGround();
    initBox(300.f, 300.f);
    initBox(400.f, 500.f);
}

void Game::initGround() {
    ground.setPosition(0, 600);
    ground.setSize(sf::Vector2f(1500.f, 200.f));
    ground.setFillColor(sf::Color::Green);
    ground.setOutlineColor(sf::Color::Black);
    ground.setOutlineThickness(1.f);
    gameObjects.push_back(ground);

    wall.setPosition(-200, 0);
    wall.setSize(sf::Vector2f(200.f, 2000.f));
    wall.setFillColor(sf::Color::Green);
    wall.setOutlineColor(sf::Color::Black);
    wall.setOutlineThickness(1.f);
    gameObjects.push_back(wall);
}

void Game::initBox(const float x, const float y) {
    auto box = sf::RectangleShape(sf::Vector2f(100, 100));
    box.setFillColor(sf::Color::Yellow);
    box.setOutlineColor(sf::Color::Black);
    box.setPosition(x, y);
    box.setOutlineThickness(1.f);
    gameObjects.push_back(box);
}

void Game::initEnemies() {
    enemy.setPosition(100, 100);
    enemy.setSize(sf::Vector2f(100.f, 100.f));
    enemy.setFillColor(sf::Color::Cyan);
    enemy.setOutlineColor(sf::Color::Green);
    enemy.setOutlineThickness(1.f);
}

Game::~Game() {
    delete this->window;
}

bool Game::running() const {
    return this->window->isOpen();
}

sf::WindowBase *Game::getWindow() const {
    return this->window;
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
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    player.shoot();
                }
            default: ;
        }
    }

    player.update(deltaTime);
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

    for (const sf::RectangleShape &object: gameObjects) {
        window->draw(object);
    }

    window->draw(player);

    for (const auto &proj: player.projectiles) {
    window->draw(proj);
    }

    view.setCenter(player.getPosition().x + 450, 300);
    window->setView(view);

    window->display();
}
