#include "Camera.h"
#include "rcamera.h"

CameraSystem gCameraSystem;

void LoadCamera()
{
    Camera tdCamera;
    //tdCamera.position = Vector3UnitZ * 100.0f;
    //tdCamera.target = Vector3Zeros;
    tdCamera.position = { 0.0f, -10.0f, 40.0f };
    tdCamera.target = Vector3Zeros;
    tdCamera.up = Vector3UnitY;
    tdCamera.fovy = 75.0f;
    tdCamera.projection = CAMERA_PERSPECTIVE;
    CameraPitch(&tdCamera, 10.0f * DEG2RAD, false, false, true);

    Camera fpCamera;
    fpCamera.position = { 0.0f, 25.0f, 2.0f };
    fpCamera.target = { 0.0f, 0.0f, 2.0f };
    fpCamera.up = Vector3UnitZ;
    fpCamera.fovy = 60.0f;
    fpCamera.projection = CAMERA_PERSPECTIVE;

    gCameraSystem.tdCamera = tdCamera;
    gCameraSystem.fpCamera = fpCamera;
    gCameraSystem.behaviour = CAM_TOP_DOWN;
    gCameraSystem.isEnabled = true;
    DisableCursor();
}

void UnloadCamera()
{
    EnableCursor();
}

void UpdateCamera()
{
    Camera* camera = GetCamera();
    if (gCameraSystem.isEnabled)
    {
        UpdateCamera(camera, CAMERA_FREE);
        // gCameraSystem.isFirstPerson ? CAMERA_FIRST_PERSON : CAMERA_FREE);
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
    {
        gCameraSystem.behaviour = gCameraSystem.behaviour == CAM_FIRST_PERSON ?
            CAM_TOP_DOWN : CAM_FIRST_PERSON;
    }
}
