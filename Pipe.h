#pragma once // Only compile once
#include "include/raylib.h"
#include <iostream>
#include <random>

class Pipe {
    public:

        float speed = 100.0f;
        int width = 32;
        int hole_offset = 128;

        bool passed_through = false;

        int top_rectangle[2];
        int bottom_rectangle[2];

        Vector2 position = Vector2();

    Pipe(Vector2 position, std::mt19937& gen);
    void render();
    void move(float dt);
};