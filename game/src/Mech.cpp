#include "Mech.h"
#include "DebugDraw.h"

void LoadMech()
{
    gMech.material = LoadMaterialDefault();
    gMech.material.maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
    gMech.torso = LoadModel("./assets/meshes/mech_torso.obj");
    gMech.legs = LoadModel("./assets/meshes/mech_legs.obj");

    gMech.pos = Vector3Zeros;
    gMech.vel = Vector3Zeros;

    gMech.rollTorso = 0.0f;
    gMech.rollLegs = 0.0f;

    gMech.moveSpeed = 10.0f;
    gMech.turnSpeed = 100.0f * DEG2RAD;
}

void UnloadMech()
{
    UnloadModel(gMech.legs);
    UnloadModel(gMech.torso);
    UnloadMaterial(gMech.material);
}

void UpdateMech(Mech& mech)
{
    float dt = GetFrameTime();

    if (IsGamepadAvailable(0))
    {
        float moveX = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X));
        float moveY = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y));
        moveX = fabsf(moveX) >= 0.25f ? moveX : 0.0f;
        moveY = fabsf(moveY) >= 0.25f ? moveY : 0.0f;
        moveY *= -1.0f;

        Vector3 vel = { moveX, moveY, 0.0f };
        vel = Vector3Normalize(vel) * mech.moveSpeed;
        mech.vel = vel;

        float turn = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X)) * -1.0f;
        if (fabsf(turn) >= 0.25f)
        {
            float dir = copysignf(1.0f, turn);
            mech.rollTorso += mech.turnSpeed * dir * dt;
        }
    }

    mech.pos += mech.vel * dt;
}

void DrawMech(const Mech& mech)
{
    Matrix rotationTorso = MatrixRotateZ(mech.rollTorso);
    Matrix rotationLegs = MatrixRotateZ(mech.rollLegs);
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);
    Matrix worldTorso = rotationTorso * translation;
    Matrix worldLegs = rotationLegs * translation;

    DrawMesh(mech.legs.meshes[0], mech.material, worldLegs);
    DrawMesh(mech.torso.meshes[0], mech.material, worldTorso);
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxes(mech.pos, MatrixRotateZ(mech.rollTorso), 25.0f, 10.0f);
}
