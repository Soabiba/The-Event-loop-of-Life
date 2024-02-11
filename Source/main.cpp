#include "raylib.h"
#include "level.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE | ConfigFlags::FLAG_VSYNC_HINT | ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "The Event Loop of Life");

    SetTargetFPS(60);

    Level level;

    level.reset();

    while (!WindowShouldClose())
    {
        // Update
        level.update();

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        level.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}