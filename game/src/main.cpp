#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

struct Models
{
    Model building;
};

struct Shaders
{
    Shader skinning;
};

struct CameraSystem
{
    Camera tdCamera;
    Camera fpCamera;
    bool isFirstPerson;
    bool isEnabled;
};

struct Mech
{
    Vector3 pos;
    float roll;
};

struct MechAnimation
{
    Model model;
    ModelAnimation* clips = nullptr;    // Array of animation clips
    int count = 0;                      // Size of animation clips array

    unsigned int index = 0;     // Index of current animation (ie 0 = idle, 1 = walk, 2 = run etc)
    unsigned int frame = 0;     // Frame of current animation
};

struct MechRenderer
{
    Material material;
    Model torso;
    Model legs;
};

Models gModels;
Shaders gShaders;
CameraSystem gCameraSystem;

Mech gMech;
MechAnimation gMechAnimation;
MechRenderer gMechRenderer;

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

void DrawMech(Mech mech)
{
    Matrix rotation = MatrixRotateZ(mech.roll);
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);
    Matrix world = rotation * translation;
    DrawMesh(gMechRenderer.legs.meshes[0], gMechRenderer.material, world);
    DrawMesh(gMechRenderer.torso.meshes[0], gMechRenderer.material, world);
}

void UpdateMechAnimation(MechAnimation& ma)
{
    int index = ma.index;

    if (IsKeyPressed(KEY_T))
        ++ma.index %= ma.count;

    else if (IsKeyPressed(KEY_G))
        ma.index = (ma.index + ma.count - 1) % ma.count;

    if (index != ma.index)
        TraceLog(LOG_INFO, TextFormat("Playing animation %i - %s\n", ma.index, ma.clips[ma.index].name));

    ModelAnimation clip = ma.clips[ma.index];
    UpdateModelAnimationBones(ma.model, clip, ma.frame++);
}

void DrawMechAnimation(const MechAnimation& ma, Matrix transform)
{
    DrawMesh(ma.model.meshes[0], ma.model.materials[1], transform);
}

void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
    
    gMechRenderer.material = LoadMaterialDefault();
    gMechRenderer.material.maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
    gMechRenderer.torso = LoadModel("./assets/meshes/mech_torso.obj");
    gMechRenderer.legs = LoadModel("./assets/meshes/mech_legs.obj");
    gModels.building = LoadModel("./assets/meshes/bld_td.obj");
    
    gShaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
    gMechAnimation.clips = LoadModelAnimations("./assets/meshes/mech.glb", &gMechAnimation.count);
    gMechAnimation.model = LoadModel("./assets/meshes/mech.glb");
    gMechAnimation.model.materials[1].shader = gShaders.skinning;
    gMechAnimation.model.materials[1].maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;

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
    UnloadModel(gModels.building);
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

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(-50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    // Original Minty Aftershave has 8x4 buildings, camera max zoom cuts off top of buildings.
    for (float y = -40.0f; y <= 40.0f; y += 20.0f)
    {
        for (float x = -80.0f; x <= 80.0f; x += 20.0f)
        {
            if (x == 0.0f && y == 0.0f) continue; // So I can see that sweet sweet mech animation!
            DrawModelWires(gModels.building, { x, y, 0.0f }, 1.0f, DARKGRAY);
        }
    }

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
