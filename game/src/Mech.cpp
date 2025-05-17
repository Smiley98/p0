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

    gMech.legsRotation = gMech.legsRotationGoal = QuaternionIdentity();
    gMech.torsoRotation = gMech.torsoRotationGoal = QuaternionIdentity();

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
            mech.legsRotationGoal = QuaternionFromEuler(0.0f, 0.0f, roll);

            Vector2 vel = dir * mech.moveSpeed;
            mech.vel.x = vel.x;
            mech.vel.y = vel.y;
        }
        
        if (Vector2Length({ aimX, aimY }) >= deadzone)
        {
            Vector2 dir = Vector2Normalize({ aimX, aimY });
            float roll = Vector2Angle(Vector2UnitY, dir);
            mech.torsoRotationGoal = QuaternionFromEuler(0.0f, 0.0f, roll);
        }
    }

    mech.legsRotation = QuaternionRotateTowards(mech.legsRotation, mech.legsRotationGoal, 250.0f * DEG2RAD * dt);
    mech.torsoRotation = QuaternionRotateTowards(mech.torsoRotation, mech.torsoRotationGoal, 100.0f * DEG2RAD * dt);

    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;
}

void DrawMech(const Mech& mech)
{
    // RHS positive rotation is CCW
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix torsoRotation = QuaternionToMatrix(mech.torsoRotation);
    Matrix legsRotation = QuaternionToMatrix(mech.legsRotation);

    Matrix torsoWorld = torsoRotation * translation;
    Matrix legsWorld = legsRotation * translation;

    DrawMesh(mech.torso.meshes[0], mech.material, torsoWorld);
    DrawMesh(mech.legs.meshes[0], mech.material, legsWorld);
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxesDebug(mech.pos, QuaternionToMatrix(mech.torsoRotation), 25.0f, 10.0f);
}
