#include "raylib.h"
#include "raymath.h"

void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);

    TraceLog(LOG_INFO, "Connor TODO - Make the map so we have a place to test our game!\n");
}

void GameCleanup()
{
    CloseWindow();
}

void GameUpdate(float dt)
{

}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText("Hello World!", 10, 10, 20, RED);
    EndDrawing();
}

int main()
{
    GameInit();

    while (!WindowShouldClose())
    {
        GameUpdate(GetFrameTime());
        GameDraw();
    }

    GameCleanup();
    return 0;
}
