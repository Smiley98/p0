#include "DebugDraw.h"
#include "rlgl.h"
#include "glad.h"
#include "Camera.h"

void DrawAxes(Vector3 position, Matrix rotation, float length, float thickness)
{
    Vector3 x = MatrixColX(rotation);
    Vector3 y = MatrixColY(rotation);
    Vector3 z = MatrixColZ(rotation);

    glDisable(GL_DEBUG_OUTPUT);
    rlSetLineWidth(thickness);
    rlEnableSmoothLines();
    BeginMode3D(*GetCamera());
    DrawLine3D(position, position + x * length, RED);
    DrawLine3D(position, position + y * length, GREEN);
    DrawLine3D(position, position + z * length, BLUE);
    EndMode3D();
    rlDisableSmoothLines();
    rlSetLineWidth(1.0f);
    glEnable(GL_DEBUG_OUTPUT);
}
