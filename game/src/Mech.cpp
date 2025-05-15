#include "Mech.h"
#include "DebugDraw.h"

static Color fColor = DARKGRAY;

void LoadMech()
{
    gMech.material = LoadMaterialDefault();
    gMech.material.maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
    gMech.torso = LoadModel("./assets/meshes/mech_torso.obj");
    gMech.legs = LoadModel("./assets/meshes/mech_legs.obj");

    gMech.pos = Vector3Zeros;
    gMech.vel = Vector3Zeros;

    gMech.currDirMove = gMech.goalDirMove = Vector2UnitY;
    gMech.currDirAim = gMech.goalDirAim = Vector2UnitY;

    gMech.moveSpeed = 10.0f;
    gMech.turnSpeed = 100.0f * DEG2RAD;

    gMech.drag = 0.05f;

    // TODO - Make QuaternionRotateTowards, then test by firing a grenade (and rotating it in its direction of motion)!
    //Vector3 x = Vector3UnitX;
    //Vector3 y = Vector3UnitY;
    //Quaternion q = QuaternionFromVector3ToVector3(x, y);
    //Vector3 axis;
    //float angle;
    //QuaternionToAxisAngle(q, &axis, &angle);
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
            mech.goalDirMove = Vector2Normalize({ moveX, moveY });
        
            // curr vs goal is just for smooth visuals. Motion is based directly on gamepad input (goal dir)
            Vector2 vel = mech.goalDirMove * mech.moveSpeed;
            mech.vel.x = vel.x;
            mech.vel.y = vel.y;
        }

        if (Vector2Length({ aimX, aimY }) >= deadzone)
        {
            Vector2 aim = Vector2Normalize({ aimX, aimY });
            mech.goalDirAim = aim;
            mech.currDirAim = Vector2RotateTowards(mech.currDirAim, mech.goalDirAim, mech.turnSpeed * dt);
        }
    }

    mech.currDirMove = Vector2RotateTowards(mech.currDirMove, mech.goalDirMove, 250.0f * DEG2RAD * dt);
    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;
}

void DrawMech(const Mech& mech)
{
    // RHS positive rotation is CCW
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix rotationTorso = MatrixRotateZ(Vector2Angle(Vector2UnitY, mech.currDirAim));
    Matrix rotationLegs = MatrixRotateZ(Vector2Angle(Vector2UnitY, mech.currDirMove));

    Matrix worldTorso = rotationTorso * translation;
    Matrix worldLegs = rotationLegs * translation;

    mech.material.maps[MATERIAL_MAP_DIFFUSE].color = fColor;
    DrawMesh(mech.torso.meshes[0], mech.material, worldTorso);
    DrawMesh(mech.legs.meshes[0], mech.material, worldLegs);
}

void DrawMechDebug(const Mech& mech)
{
    //float aimAngle = Vector2Angle(Vector2UnitY, mech.currDirAim);
    //DrawAxesDebug(mech.pos, MatrixRotateZ(aimAngle), 25.0f, 10.0f);

    // Must respect RHS coordinates (since using "up" as local_z, must supply vectors accordingly)!
    Vector3 aim = { mech.currDirAim.x, mech.currDirAim.y, 0.0f };
    DrawAxesDebug(mech.pos, MatrixLookRotation(Vector3UnitZ, aim), 25.0f, 10.0f);
}

// If I want to pair text with lines, I'll need to make something like
// struct MechDebug and update moveAngleDiff and aimAngleDiff there, then separate rendering between 2D vs 3D

// Still, DrawMesh recomputes mvp, and resends m, v, p (and mvp). Begin/End aren't very expensive.
// Overall more correct to do debug-related updates within update.

// Original grenade implementation lerps its direction towards its velocity (all physics, no quadratic formula)
// Original did so by using Frenet-frame to orientate in direction of motion ie rotation = Frenet(Lerp(dir, vel, 0.01))
// I'll most likely need something similar if I'm trying to point an object in its direction of motion
// (motion is a direction, not a rotation, so I can't use QuaternionRotateTowards).

// Launch solution should be as simple as local-up = Cross(projectile_forward, mech_right)
// where projectile_forward = mech_forward * launch_elevation
