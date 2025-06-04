#include "Pipe.h"
#include <random>
#include <iostream>

Pipe::Pipe(Vector2 Aposition, std::mt19937& gen) {
    position = Aposition;

    // Generating random numbers (gen is the engine used to generate them)
    std::uniform_int_distribution<> midpointDist(225 - hole_offset, 225 + hole_offset);
    std::uniform_int_distribution<> sizeDist(50, 100); // Size each side
    int midpoint = midpointDist(gen);
    int size = sizeDist(gen);

    // Calculate both rectangles needed for the pipe and save (y, height) into int arrays
    int hole[2] = {
        midpoint - size,
        midpoint + size
    };

    top_rectangle[0] = 0;
    top_rectangle[1] = hole[0];

    bottom_rectangle[0] = hole[1];
    bottom_rectangle[1] = 450 - hole[1];
}

void Pipe::render() {
    DrawRectangle(position.x, top_rectangle[0], width, top_rectangle[1], LIME);
    DrawRectangle(position.x, bottom_rectangle[0], width, bottom_rectangle[1], LIME);
}

void Pipe::move(float dt) {
    position.x -= speed * dt;
}