#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

struct Player : Entity {
public:
    // Constructor
    explicit Player(Game *game);

    void update(float deltaTime) override;
};


#endif //PLAYER_H
