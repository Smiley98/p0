#include "raylib.h"
#include "raymath.h"

void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
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
    //this is proof that josh can make edits to the code and push
    GameInit();

    while (!WindowShouldClose())
    {
        GameUpdate(GetFrameTime());
        GameDraw();
    }

    GameCleanup();
    return 0;
}
