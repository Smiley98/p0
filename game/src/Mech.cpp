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

    gMech.moveAngle = gMech.moveAngleGoal = 0.0f;
    gMech.aimAngle = gMech.aimAngleGoal = 0.0f;

    gMech.moveSpeed = 10.0f;
    gMech.turnSpeed = 100.0f * DEG2RAD;

    gMech.drag = 0.05f;

    // Theory test success -- chooses optimal angle of -170 instead of 190!
    Quaternion a = QuaternionFromEuler(0.0f, 0.0f, 0.0f * DEG2RAD);
    Quaternion b = QuaternionFromEuler(0.0f, 0.0f, 190.0f * DEG2RAD);
    Quaternion c = QuaternionRotateTowards(a, b, 180.0f * DEG2RAD);
    Vector3 angles = QuaternionToEuler(c) * RAD2DEG;

    // TODO - Make Mech simple - rotations will only happen about Z, so just store an angle instead of a quaternion.
    // Use spherical lerp to rotate towards angles!
    // Can even make MechDirection function - { cosf(angle), sinf(angle), 0.0f }. Forget 3d internal representation!
    // Once mech is simplified, delegate grenade launching task!
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
        Vector3 move{ moveX, moveY, 0.0f };
        Vector3 aim{ aimX, aimY, 0.0f };

        if (Vector3Length(move) >= deadzone)
        {
            move = Vector3Normalize(move); 
            mech.vel = move * mech.moveSpeed;           // Mech logical move is instantaneous
            mech.moveAngleGoal = p0Vector3Angle(move);  // Mech visual move is gradual
        }
        
        if (Vector3Length(aim) >= deadzone)
        {
            aim = Vector3Normalize(aim);
            mech.aimAngleGoal = p0Vector3Angle(aim);

            float aimCurr = mech.aimAngle;
            float aimGoal = mech.aimAngleGoal;
            Vector2 aimDir = Vector2RotateTowards(p0Vector2Direction(aimCurr), p0Vector2Direction(aimGoal), mech.turnSpeed * dt);
            mech.aimAngle = p0Vector2Angle(aimDir);
        }
    }

    float moveCurr = mech.moveAngle;
    float moveGoal = mech.moveAngleGoal;
    Vector2 moveDir = Vector2RotateTowards(p0Vector2Direction(moveCurr), p0Vector2Direction(moveGoal), 250.0f * DEG2RAD * dt);
    mech.moveAngle = p0Vector2Angle(moveDir);

    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;
}

void DrawMech(const Mech& mech)
{
    // RHS positive rotation is CCW
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix rotationTorso = MatrixRotateZ(mech.aimAngle);
    Matrix rotationLegs = MatrixRotateZ(mech.moveAngle);

    Matrix worldTorso = rotationTorso * translation;
    Matrix worldLegs = rotationLegs * translation;

    mech.material.maps[MATERIAL_MAP_DIFFUSE].color = fColor;
    DrawMesh(mech.torso.meshes[0], mech.material, worldTorso);
    DrawMesh(mech.legs.meshes[0], mech.material, worldLegs);
}

void DrawMechDebug(const Mech& mech)
{
    Vector3 aim = p0Vector3Direction(mech.aimAngle);
    DrawAxesDebug(mech.pos, p0MatrixLookRotation(aim), 25.0f, 10.0f);
}
