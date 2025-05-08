#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

struct Assets
{
    Model building;
};

struct CameraSystem
{
    Camera tdCamera;
    Camera fpCamera;
    bool isFirstPerson;
    bool isEnabled;
};

Assets gAssets;
CameraSystem gCameraSystem;

void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
    
    gAssets.building = LoadModel("./assets/meshes/bld_td.obj");

    Camera tdCamera;
    tdCamera.position = Vector3UnitY * 100.0f;
    tdCamera.target = Vector3Zeros;
    tdCamera.up = Vector3UnitZ * -1.0f;
    tdCamera.fovy = 75.0f;
    tdCamera.projection = CAMERA_PERSPECTIVE;

    Camera fpCamera;
    fpCamera.position = { 0.0f, 2.0f, 0.0f };
    fpCamera.target = { 0.0f, 2.0f, -1.0f };
    fpCamera.up = Vector3UnitY;
    fpCamera.fovy = 60.0f;
    fpCamera.projection = CAMERA_PERSPECTIVE;

    gCameraSystem.tdCamera = tdCamera;
    gCameraSystem.fpCamera = fpCamera;
    gCameraSystem.isFirstPerson = false;
    gCameraSystem.isEnabled = true;

    DisableCursor();
}

void GameCleanup()
{
    UnloadModel(gAssets.building);
    CloseWindow();
}

void GameUpdate(float dt)
{
    if (gCameraSystem.isEnabled)
    {
        UpdateCamera(
            gCameraSystem.isFirstPerson ? &gCameraSystem.fpCamera : &gCameraSystem.tdCamera,
            gCameraSystem.isFirstPerson ? CAMERA_FIRST_PERSON : CAMERA_FREE);
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
}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(gCameraSystem.isFirstPerson ? gCameraSystem.fpCamera : gCameraSystem.tdCamera);

    rlPushMatrix();
    rlTranslatef(50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    rlPushMatrix();
    rlTranslatef(-50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    // Original Minty Aftershave has 8x4 buildings, camera max zoom cuts off top of buildings.
    for (float z = -40.0f; z <= 40.0f; z += 20.0f)
    {
        for (float x = -80.0f; x <= 80.0f; x += 20.0f)
        {
            DrawModel(gAssets.building, { x, 0.0f, z }, 1.0f, DARKGRAY);
        }
    }

    EndMode3D();
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
// 
// Vector3 MatrixRight(Matrix m)
//{
//    return { m.m0, m.m1, m.m2 };
//}
//
//Vector3 MatrixUp(Matrix m)
//{
//    return { m.m4, m.m5, m.m6 };
//}
//
//Vector3 MatrixForward(Matrix m)
//{
//    return { m.m8, m.m9, m.m10 };
//}
// 
//Matrix view = GetCameraMatrix(gCameraSystem.tdCamera);
//Matrix camera = MatrixInvert(view);
//
//Vector3 x = MatrixRight(camera);    //  1,  0,  0
//Vector3 y = MatrixUp(camera);       //  0,  0, -1
//Vector3 z = MatrixForward(camera);  //  0,  1,  0
