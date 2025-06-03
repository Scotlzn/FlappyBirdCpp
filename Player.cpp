#include "Player.h"

// Constructor
Player::Player(float xp, float yp) {
    x = xp;
    y = yp;
}

void Player::render() {
    // DrawRectangle(int posX, int posY, int width, int height, Color color);
    DrawRectangle(x, y, 32, 32, Color{225, 225, 0, 255});
}

void Player::move(float dt) {
    velocity += gravity * dt;
    if (velocity > terminal_velocity) velocity = terminal_velocity; // terminal velocity
    y += velocity * dt;
}