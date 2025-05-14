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

    gMech.currDirMove = gMech.goalDirMove = Vector2UnitY;
    gMech.currDirAim = gMech.goalDirAim = Vector2UnitY;

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
    float aimAngle = Vector2Angle(Vector2UnitY, mech.currDirAim);
    DrawAxes(mech.pos, MatrixRotateZ(aimAngle), 25.0f, 10.0f);
}
