#include "Mech.h"
#include "DebugDraw.h"

static Color fColor = RED;

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

    gMech.drag = 0.05f;
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
        const float t = 0.5f; // My Xbox controller has HUGE deadzones xD
        float moveX = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X));
        float moveY = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y));
        moveX = fabsf(moveX) >= t ? moveX : 0.0f;
        moveY = fabsf(moveY) >= t ? moveY : 0.0f;
        moveY *= -1.0f;

        // TODO - Make rotation gradual instead of instantaneous
        if (Vector2Length({ moveX, moveY }) >= t)
        {
            Vector2 dir = Vector2Normalize({ moveX, moveY });
            mech.rollLegs = Vector2Angle(Vector2UnitY, dir);

            Vector2 vel = dir * mech.moveSpeed;
            mech.vel.x = vel.x;
            mech.vel.y = vel.y;
            
            fColor = RED;
        }
        else
            fColor = GREEN;

        float turn = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X)) * -1.0f;
        if (fabsf(turn) >= t)
        {
            float dir = copysignf(1.0f, turn);
            mech.rollTorso += mech.turnSpeed * dir * dt;
        }
    }

    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;
}

void DrawMech(const Mech& mech)
{
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix rotationTorso = MatrixRotateZ(mech.rollTorso);
    Matrix rotationLegs = MatrixRotateZ(mech.rollLegs);

    Matrix worldTorso = rotationTorso * translation;
    Matrix worldLegs = rotationLegs * translation;

    mech.material.maps[MATERIAL_MAP_DIFFUSE].color = fColor;
    DrawMesh(mech.torso.meshes[0], mech.material, worldTorso);
    DrawMesh(mech.legs.meshes[0], mech.material, worldLegs);

    //Vector2Angle(v1, v2) where v1 is basis ie right or up, and v2 is direction
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxes(mech.pos, MatrixRotateZ(mech.rollTorso), 25.0f, 10.0f);
}
