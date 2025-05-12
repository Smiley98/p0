#pragma once
#include "raylib.h"
#include "raymath.h"

struct CameraSystem
{
    Camera tdCamera;
    Camera fpCamera;
    bool isFirstPerson;
    bool isEnabled;
};

extern CameraSystem gCameraSystem;

inline Camera* GetCamera()
{
    Camera* camera = gCameraSystem.isFirstPerson ?
        &gCameraSystem.fpCamera : &gCameraSystem.tdCamera;
    return camera;
}

void LoadCamera();
void UnloadCamera();

void UpdateCamera();