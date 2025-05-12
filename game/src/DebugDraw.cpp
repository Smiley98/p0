#include "DebugDraw.h"
#include "rlgl.h"
#include "glad.h"
#include "Camera.h"

void DrawAxes(Matrix rotation, float length, float thickness)
{
    Vector3 x = MatrixColX(rotation);
    Vector3 y = MatrixColY(rotation);
    Vector3 z = MatrixColZ(rotation);

    glDisable(GL_DEBUG_OUTPUT);
    rlSetLineWidth(thickness);
    rlEnableSmoothLines();
    BeginMode3D(*GetCamera());
    DrawLine3D(Vector3Zeros, x * length, RED);
    DrawLine3D(Vector3Zeros, y * length, GREEN);
    DrawLine3D(Vector3Zeros, z * length, BLUE);
    EndMode3D();
    rlDisableSmoothLines();
    rlSetLineWidth(1.0f);
    glEnable(GL_DEBUG_OUTPUT);
}
