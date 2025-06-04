#include "raylib.h"
#include "raymath.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"
#include "DebugDraw.h"

#include "Scene.h"
#include "World.h"

void AppLoad()
{
    LoadCamera();
    LoadMeshes();
    LoadShaders();
    LoadTextures();
    LoadAudio();
}

void AppUnload()
{
    UnloadAudio();
    UnloadTextures();
    UnloadShaders();
    UnloadMeshes();
    UnloadCamera();
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(144);
    AppLoad();
    Scene::Load(SCENE_DEV_MAP);

    while (!WindowShouldClose())
    {
        Scene::Update();
        BeginDrawing();
        ClearBackground(BLACK);
        Scene::Draw();
#ifdef DEBUG
        Scene::DrawDebug();
#endif
        Scene::DrawGui();
        EndDrawing();
    }

    Scene::Unload();
    AppUnload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
