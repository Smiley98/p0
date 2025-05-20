#include "raylib.h"
#include "raymath.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "DebugDraw.h"

#include "World.h"

void AppLoad()
{
    LoadCamera();
    LoadMeshes();
    LoadShaders();
}

void AppUnload()
{
    UnloadShaders();
    UnloadMeshes();
    UnloadCamera();
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
    AppLoad();

    World world;
    LoadWorld(world);
    while (!WindowShouldClose())
    {
        UpdateCamera();
        UpdateWorld(world);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawWorld(world);
        DrawWorldDebug(world);

        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadWorld(world);
    AppUnload();

    CloseWindow();
    return 0;
}
