#pragma once // Only compile once
#include "include/raylib.h"

class Player {

    public:
        float x;
        float y;
        float velocity = 0.0f;
        float terminal_velocity = 600.0f;

        const float gravity = 250.0f;
        const float jump_height = 250.0f;

        Player(float x, float y);

        void render();
        void move(float dt);

};