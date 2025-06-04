#include "include/raylib.h"
#include <cstdio> // for sprintf
#include <iostream> // for std
#include <vector> // for std::vector
#include "Player.h"
#include "Pipe.h"
#include <random>
#include "JsonLoader.h"

// g++ $(Get-ChildItem -Name -Filter *.cpp) -o game.exe -O1 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm; if ($?) { ./game.exe }
bool RectangleCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

bool JumpButton() {
    if (IsKeyPressed(KEY_SPACE)) return true; // Check spacebar
    if (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { 
        return true; // Check if gamepad 0 is connected and A button is pressed
    }
    return false;
}

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    const int time_between_pipes = 2;

    // SetConfigFlags(FLAG_VSYNC_HINT); // Enable Vsync

    InitWindow(screenWidth, screenHeight, "Flappy Bird C++");

    // Set program to run at 60 frames-per-second
    SetTargetFPS(60);

    JsonLoader json_loader; // For high-score

    Player player(400 - 16, 32);

    int score = 0;
    int high_score = json_loader.LoadJson();
    float pipe_timer = 0.0f;
    bool death_processed = false;

    // Similar to a list (able to change size)
    std::vector<Pipe> pipes;

    // Random generator stuff
    std::random_device rd;  // Seed random generator
    std::mt19937 gen(rd()); // Apply using Mersenne Twister engine

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        // Only run game when player is alive
        if (player.alive) { 

            // Delta time in seconds
            float dt = GetFrameTime();

            // Player Jump
            if (JumpButton()) {
                player.velocity = -player.jump_height;
            }

            // Pipe spawning
            if (pipe_timer >= time_between_pipes) {
                Vector2 start_position = {screenWidth, 0};
                pipes.push_back(Pipe(start_position, gen));
                pipe_timer = 0.0f;
            }

            // Update player
            player.move(dt);

            // Kill the player if offscreen
            if (player.y > screenHeight) {
                player.alive = false;
            }

            // Update pipes
            for (int i = (int)pipes.size() - 1; i >= 0; i--) {
                pipes[i].move(dt);
                Pipe current_pipe = pipes[i];
                
                // General collision with a pipe
                if (RectangleCollision(player.x, player.y, 32, 32, current_pipe.position.x, 0, current_pipe.width, screenHeight)) {

                    // Collision with top section of the pipe
                    if (RectangleCollision(player.x, player.y, 32, 32, current_pipe.position.x, current_pipe.top_rectangle[0], current_pipe.width, current_pipe.top_rectangle[1])) {
                        player.alive = false;

                    // Collision with bottom section of the pipe
                    } else if (RectangleCollision(player.x, player.y, 32, 32, current_pipe.position.x, current_pipe.bottom_rectangle[0], current_pipe.width, current_pipe.bottom_rectangle[1])) {
                        player.alive = false;

                    // Passed through pipe, increase score
                    } else if (!current_pipe.passed_through) {
                        pipes[i].passed_through = true;
                        score++;
                    }

                }

                // Remove pipe when offscreen
                if (current_pipe.position.x < -current_pipe.width) {
                    // takes an iterator (pointer)
                    pipes.erase(pipes.begin() + i); // .begin() gives the iterator of the first element 
                }
            }
            pipe_timer += dt;

            // Process death ._., update high score
            if (!player.alive && !death_processed) {
                death_processed = true;
                if (score > high_score) {
                    high_score = score;
                    json_loader.SaveJson(score);
                }
            }
        }

        BeginDrawing();

            ClearBackground(SKYBLUE);

            player.render();

            // Render pipes
            for (int i = (int)pipes.size() - 1; i >= 0; i--) {
                pipes[i].render();
            }

            // Grey screen out when player dies
            if (!player.alive) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Color{130, 130, 130, 200});

                // Death UI
                DrawText("You died lol how are u so ass", 250, 170, 20, RED);
                DrawText(TextFormat("Final score: %d", score), 325, 220, 20, RED);
                DrawText(TextFormat("High score: %d", high_score), 325, 250, 20, RED);
            
            // Only render game specific UI when game is active
            } else { 

                // Game UI
                DrawText(TextFormat("Score: %d", score), 5, 5, 20, GRAY);
                DrawText(TextFormat("High score: %d", high_score), 5, 25, 20, GRAY);

                // DrawText(TextFormat("Number of pipes: %d", (int)pipes.size()), 190, 200, 20, LIGHTGRAY);
                // DrawText(TextFormat("Player Alive?: %s", player.alive ? "true" : "false"), 190, 250, 20, LIGHTGRAY);
                // DrawText(TextFormat("dt: %.4f", GetFrameTime()), 10, 40, 20, BLACK);
                // DrawFPS(0, 0);

            }

        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}
