#include "include/raylib.h"
#include <cstdio> // for sprintf
#include <iostream> // for std
#include <vector> // for std::vector
#include "Player.h"
#include "Pipe.h"
#include <random>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    const int time_between_pipes = 2;

    // SetConfigFlags(FLAG_VSYNC_HINT); // Enable Vsync

    InitWindow(screenWidth, screenHeight, "Flappy Bird C++!");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Player player(32, 32);

    char buffer[16];
    sprintf(buffer, "%.2f", player.velocity);

    float pipe_timer = 0.0f;

    // Similar to a list (able to change size)
    std::vector<Pipe> pipes;

    std::random_device rd;  // Seed random generator
    std::mt19937 gen(rd()); // Apply using Mersenne Twister engine

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        float dt = GetFrameTime(); // Delta time in seconds

        if (IsKeyPressed(KEY_SPACE)) {
            player.velocity = -player.jump_height;
        }

        if (pipe_timer >= time_between_pipes) {
            // Spawn pipe
            Vector2 start_position = {screenWidth, 0};
            pipes.push_back(Pipe(start_position, gen));
            pipe_timer = 0.0f;
        }

        player.move(dt);

        // Update pipes
        for (int i = (int)pipes.size() - 1; i >= 0; i--) {
            pipes[i].move(dt);
            if (pipes[i].position.x < -32) {
                // Remove pipe when offscreen, takes an iterator (pointer)
                pipes.erase(pipes.begin() + i); // .begin() gives the iterator of the first element 
            }
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            player.render();

            // Render pipes
            for (int i = (int)pipes.size() - 1; i >= 0; i--) {
                pipes[i].render();
            }

            DrawText(TextFormat("Number of pipes: %d", (int)pipes.size()), 190, 200, 20, LIGHTGRAY);
            DrawFPS(0, 0);
            DrawText(TextFormat("dt: %.4f", GetFrameTime()), 10, 40, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------

        pipe_timer += dt;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}