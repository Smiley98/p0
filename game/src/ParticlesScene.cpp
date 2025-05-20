#include "ParticlesScene.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "rlgl.h"

Camera3D camera = { 0 };
ParticleEmitter emitter;
Texture2D TEX;

void ParticlesScene::OnLoad()
{
    unsigned char pixels[4] = { 255, 255, 255, 255 };
    unsigned int id = rlLoadTexture(pixels, 1, 1, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
    TEX.id = id;
    TEX.width = 1;
    TEX.height = 1;
    TEX.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    TEX.mipmaps = 1;

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

void ParticlesScene::OnUnload()
{
}

void ParticlesScene::OnStart()
{
}

void ParticlesScene::OnStop()
{
}

void ParticlesScene::OnUpdate()
{
    emitter.Update(GetFrameTime());
}

void ParticlesScene::OnDraw()
{
    BeginMode3D(camera);
    emitter.Draw(camera);
    EndMode3D();
}

void ParticlesScene::OnDrawDebug()
{
}

void ParticlesScene::OnDrawGui()
{
}
