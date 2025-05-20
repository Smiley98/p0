#include "Mech.h"
#include "DebugDraw.h"
#include "Meshes.h"

void CreateMech(Mech* mech, int player)
{
    float roll = player % 2 == 0 ? 0.0f : PI;
    Quaternion rotation = QuaternionFromEuler(0.0f, 0.0f, roll);

    mech->legs_rotation = rotation;
    mech->torso_rotation = rotation;
    mech->legs_rotation_goal = rotation;
    mech->torso_rotation_goal = rotation;

    mech->material = LoadMaterialDefault();
    mech->material.maps[MATERIAL_MAP_DIFFUSE].color = GRAY;

    mech->player = player;
}

void DestroyMech(Mech* mech)
{
    UnloadMaterial(mech->material);
}

void UpdateMech(Mech& mech)
{
    float dt = GetFrameTime();

    if (IsGamepadAvailable(mech.player))
    {
        const float deadzone = 0.5f;
        float moveX = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X));
        float moveY = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y));
        float aimX = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X));
        float aimY = (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y));
        moveX = fabsf(moveX) >= deadzone ? moveX : 0.0f;
        moveY = fabsf(moveY) >= deadzone ? moveY : 0.0f;
        aimX = fabsf(aimX) >= deadzone ? aimX : 0.0f;
        aimY = fabsf(aimY) >= deadzone ? aimY : 0.0f;
        moveY *= -1.0f;
        aimY *= -1.0f;
        
        if (Vector2Length({ moveX, moveY }) >= deadzone)
        {
            Vector2 dir = Vector2Normalize({ moveX, moveY });
            float roll = Vector2Angle(Vector2UnitY, dir);
            mech.legs_rotation_goal = QuaternionFromEuler(0.0f, 0.0f, roll);

            Vector2 vel = dir * mech.move_speed;
            mech.vel.x = vel.x;
            mech.vel.y = vel.y;
        }
        
        if (Vector2Length({ aimX, aimY }) >= deadzone)
        {
            Vector2 dir = Vector2Normalize({ aimX, aimY });
            float roll = Vector2Angle(Vector2UnitY, dir);
            mech.torso_rotation_goal = QuaternionFromEuler(0.0f, 0.0f, roll);
        }
    }

    mech.legs_rotation = QuaternionRotateTowards(mech.legs_rotation, mech.legs_rotation_goal, 250.0f * DEG2RAD * dt);
    mech.torso_rotation = QuaternionRotateTowards(mech.torso_rotation, mech.torso_rotation_goal, 100.0f * DEG2RAD * dt);

    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;
}

void DrawMech(const Mech& mech)
{
    // RHS positive rotation is CCW
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix torso_rotation = QuaternionToMatrix(mech.torso_rotation);
    Matrix legs_rotation = QuaternionToMatrix(mech.legs_rotation);

    Matrix torsoWorld = torso_rotation * translation;
    Matrix legsWorld = legs_rotation * translation;

    DrawMesh(*g_meshes.mech_torso, mech.material, torsoWorld);
    DrawMesh(*g_meshes.mech_legs, mech.material, legsWorld);
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxesDebug(mech.pos, QuaternionToMatrix(mech.torso_rotation), 25.0f, 10.0f);
}
