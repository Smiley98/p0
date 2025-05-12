#pragma once
#include "raylib.h"
#include "raymath.h"

enum CameraBehaviour
{
    CAM_TOP_DOWN,
    CAM_FIRST_PERSON,
    CAM_FOLLOW
};

struct CameraSystem
{
    Camera tdCamera;
    Camera fpCamera;
    Camera flCamera;
    CameraBehaviour behaviour;
    bool isEnabled;
};

extern CameraSystem gCameraSystem;

inline Camera* GetCamera()
{
    Camera* camera = nullptr;
    switch (gCameraSystem.behaviour)
    {
    case CAM_TOP_DOWN:
        camera = &gCameraSystem.tdCamera;
        break;

    case CAM_FIRST_PERSON:
        camera = &gCameraSystem.fpCamera;
        break;

    case CAM_FOLLOW:
        camera = &gCameraSystem.flCamera;
        break;
    }
    return camera;
}

void LoadCamera();
void UnloadCamera();

void UpdateCamera();