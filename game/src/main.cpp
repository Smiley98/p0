#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "Shaders.h"
#include "Camera.h"
#include "DebugDraw.h"

#include "Mech.h"
#include "MechAnimation.h"
#include "Buildings.h"

Mech gMech;
MechAnimation gMechAnimation;

void GameLoad()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
    
    LoadShaders();
    LoadBuildings();

    LoadMech();
    LoadMechAnimation();

    LoadCamera();
}

void GameUnload()
{
    UnloadCamera();

    UnloadMechAnimation();
    UnloadMech();

    UnloadBuildings();
    UnloadShaders();

    CloseWindow();
}

void GameUpdate(float dt)
{
    UpdateCamera();
    UpdateMech(gMech);
    UpdateMechAnimation(gMechAnimation);
}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(*GetCamera());

    DrawBuildings();
    //DrawMechAnimation(gMechAnimation, MatrixIdentity());
    DrawMech(gMech);
    EndMode3D();
    
    DrawAxes(MatrixRotateZ(gMech.roll), 25.0f, 10.0f);
    DrawFPS(10, 10);
    EndDrawing();
}

int main()
{
    GameLoad();

    while (!WindowShouldClose())
    {
        GameUpdate(GetFrameTime());
        GameDraw();
    }

    GameUnload();
    return 0;
}

// TODO - Decide on skeletal animation vs morph target animation
