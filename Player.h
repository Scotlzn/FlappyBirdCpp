#pragma once // Only compile once
#include "include/raylib.h"

class Player {

    public:

        const float gravity = 250.0f;
        const float jump_height = 250.0f;

        float x;
        float y;
        float velocity = 0.0f;
        float terminal_velocity = 600.0f;

        bool alive = true;

        Player(float x, float y);

        void render();
        void move(float dt);

};