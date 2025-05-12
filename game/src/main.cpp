#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "Shaders.h"

#include "Mech.h"
#include "MechAnimation.h"
#include "Buildings.h"

Mech gMech;
MechAnimation gMechAnimation;

struct CameraSystem
{
    Camera tdCamera;
    Camera fpCamera;
    bool isFirstPerson;
    bool isEnabled;
};

CameraSystem gCameraSystem;

Camera* GetCamera()
{
    Camera* camera = gCameraSystem.isFirstPerson ? &gCameraSystem.fpCamera : &gCameraSystem.tdCamera;
    return camera;
}

Vector3 MatrixColX(Matrix m)
{
    return { m.m0, m.m1, m.m2 };
}

Vector3 MatrixColY(Matrix m)
{
    return { m.m4, m.m5, m.m6 };
}

Vector3 MatrixColZ(Matrix m)
{
    return { m.m8, m.m9, m.m10 };
}

void DrawAxes(Matrix rotation, float length, float thickness = 1.0f)
{
    Vector3 x = MatrixColX(rotation);
    Vector3 y = MatrixColY(rotation);
    Vector3 z = MatrixColZ(rotation);

    rlSetLineWidth(thickness);
    rlEnableSmoothLines();
    BeginMode3D(*GetCamera());
    DrawLine3D(Vector3Zeros, x * length, RED);
    DrawLine3D(Vector3Zeros, y * length, GREEN);
    DrawLine3D(Vector3Zeros, z * length, BLUE);
    EndMode3D();
    rlDisableSmoothLines();
    rlSetLineWidth(1.0f);
}

void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
    
    LoadShaders();
    LoadBuildings();

    LoadMech();
    LoadMechAnimation();

    Camera tdCamera;
    tdCamera.position = Vector3UnitZ * 100.0f;
    tdCamera.target = Vector3Zeros;
    tdCamera.up = Vector3UnitY;
    tdCamera.fovy = 75.0f;
    tdCamera.projection = CAMERA_PERSPECTIVE;

    Camera fpCamera;
    fpCamera.position = { 0.0f, 10.0f, 2.0f };
    fpCamera.target = { 0.0f, 0.0f, 2.0f };
    fpCamera.up = Vector3UnitZ;
    fpCamera.fovy = 60.0f;
    fpCamera.projection = CAMERA_PERSPECTIVE;

    gCameraSystem.tdCamera = tdCamera;
    gCameraSystem.fpCamera = fpCamera;
    gCameraSystem.isFirstPerson = false;
    gCameraSystem.isEnabled = true;
    DisableCursor();

    gMech.pos = Vector3Zeros;
    gMech.roll = 0.0f;
}

void GameCleanup()
{
    UnloadMechAnimation();
    UnloadMech();

    UnloadBuildings();
    UnloadShaders();

    CloseWindow();
}

void GameUpdate(float dt)
{
    Camera* camera = GetCamera();
    if (gCameraSystem.isEnabled)
    {
        UpdateCamera(camera, CAMERA_FREE);// gCameraSystem.isFirstPerson ? CAMERA_FIRST_PERSON : CAMERA_FREE);
    }

    if (IsKeyPressed(KEY_V))
    {
        gCameraSystem.isEnabled = !gCameraSystem.isEnabled;
        if (gCameraSystem.isEnabled)
            DisableCursor();
        else
            EnableCursor();
    }

    if (IsKeyPressed(KEY_C))
        gCameraSystem.isFirstPerson = !gCameraSystem.isFirstPerson;

    UpdateMechAnimation(gMechAnimation);
}

void GameDraw()
{
    Camera* camera = GetCamera();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(*camera);

    DrawBuildings();
    //DrawMechAnimation(gMechAnimation, MatrixIdentity());
    gMech.roll = GetTime() * 100.0f * DEG2RAD;
    DrawMech(gMech);
    EndMode3D();
    
    DrawAxes(MatrixRotateZ(gMech.roll), 25.0f, 10.0f);
    DrawFPS(10, 10);
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

// TODO - Determine world size - either 100x100 or 1000x1000 (original TD building is 91.5 x 91.5 x 438, seems a little large)...
// Will probably go with something smaller like 100x100 for improved precision, plus I'm re-exporting assets to RHS so might as well change dimensions too!

// Right-handed system:
// col[0] = right
// col[1] = up
// col[2] = forward "+z = out of the screen", "-z = into the screen"

// Validate top-down camera axes:
//Matrix view = GetCameraMatrix(gCameraSystem.tdCamera);
//Matrix camera = MatrixInvert(view);
//
//Vector3 x = MatrixRight(camera);    //  1,  0,  0
//Vector3 y = MatrixUp(camera);       //  0,  0, -1
//Vector3 z = MatrixForward(camera);  //  0,  1,  0

// CAMERA_FIRST_PERSON only works if ground plane is XZ
//CameraMode GetCameraMode()
//{
//    return gCameraSystem.isFirstPerson ? CAMERA_FIRST_PERSON : CAMERA_FREE;
//}
