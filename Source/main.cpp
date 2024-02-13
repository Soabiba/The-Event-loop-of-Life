#include "raylib.h"
#include "level.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE | ConfigFlags::FLAG_VSYNC_HINT | ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "The Event Loop of Life");

    Flock flock;
    flock.initialize(20, screenWidth, screenHeight); // Initialize flock with 20 crows

    SetTargetFPS(60);

    Level level;

    level.reset();

    while (!WindowShouldClose())
    {
        // Update
        level.update();
        flock.update();

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        level.draw();
        flock.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}