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

void GameDraw3D()
{
    BeginMode3D(*GetCamera());
        DrawBuildings();
        //DrawMechAnimation(gMechAnimation, MatrixIdentity());
        DrawMech(gMech);
        DrawMechDebug(gMech);
    EndMode3D();
}

void GameDraw2D()
{
    DrawFPS(10, 10);
}

int main()
{
    GameLoad();

    while (!WindowShouldClose())
    {
        GameUpdate(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        GameDraw3D();
        GameDraw2D();
        EndDrawing();
    }

    GameUnload();
    return 0;
}

// TODO - Decide on skeletal animation vs morph target animation
// TODO - Decide whether to wrap individual draw calls in BeginMode3D/EndMode3D vs
//        separate 2D vs 3D rendering entirely
// Probably the latter because we should be thinking in systems rather than objects.
// Mechs + Buildings + Particles in 3D. UI in 2D.