#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "ParticleEmitter.h"


Camera3D camera = { 0 };
ParticleEmitter emitter;
Texture2D TEX;

void GameInit()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(144);
    TEX = LoadTexture("game/Assets/Textures/Particles/T_Particle_Test.png");
    if (TEX.id == 0) TraceLog(LOG_ERROR, "Failed to load particle texture!");

    //Obviously this will be changed
    camera.position = { 0.0f, 0.0f, 10.0f };
    camera.target = { 0.0f, -4.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 110.0f;
    camera.projection = CAMERA_PERSPECTIVE;

	//I want ui to set this up and then save it to a file or something
	//so in game you can just load the file and it will set up the emitter and whatever
    emitter.position = { 0.0f, -1.0f, -2.0f };
    emitter.shape = ParticleEmitter::Shape::SPHERE;
    emitter.spawnRate = 20.0f;
    emitter.particleSpeed = 1.5f;
    emitter.direction = { 0.5f, 1.0f, 0.0f };
    emitter.particleSize = 0.3f;
    emitter.lifetime = 3.0f;
	emitter.radius = 4;
	//emitter.coneAngle = 45;
	emitter.startColor = { 255, 255, 255, 255 };
    

}

void GameCleanup()
{
    CloseWindow();
}

void GameUpdate(float dt)
{

    //maybe update all emiters or something
    emitter.Update(dt);
}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(BLANK);

    BeginMode3D(camera);

    //same here
    emitter.Draw(camera);

    EndMode3D();

    DrawText("Hello Connor!", 10, 10, 20, RED);
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
