#include "Mech.h"
#include "DebugDraw.h"
#include "Meshes.h"
// Normalize vector
Vector3 Normalize(Vector3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len < 0.0001f) return  { 0, 0, 1 }; // default forward if zero velocity
    return  { v.x / len, v.y / len, v.z / len };
}

// Cross product
Vector3 Cross(Vector3 a, Vector3 b) {
    return  {
        a.y* b.z - a.z * b.y,
            a.z* b.x - a.x * b.z,
            a.x* b.y - a.y * b.x
    };
}

// Quaternion from rotation matrix
Quaternion QuatFromMatrix(Matrix m) {
    Quaternion q;
    float trace = m.m0 + m.m5 + m.m10;

    if (trace > 0) {
        float s = sqrtf(trace + 1.0f) * 2;
        q.w = 0.25f * s;
        q.x = (m.m9 - m.m6) / s;
        q.y = (m.m2 - m.m8) / s;
        q.z = (m.m4 - m.m1) / s;
    }
    else if (m.m0 > m.m5 && m.m0 > m.m10) {
        float s = sqrtf(1.0f + m.m0 - m.m5 - m.m10) * 2;
        q.w = (m.m9 - m.m6) / s;
        q.x = 0.25f * s;
        q.y = (m.m1 + m.m4) / s;
        q.z = (m.m2 + m.m8) / s;
    }
    else if (m.m5 > m.m10) {
        float s = sqrtf(1.0f + m.m5 - m.m0 - m.m10) * 2;
        q.w = (m.m2 - m.m8) / s;
        q.x = (m.m1 + m.m4) / s;
        q.y = 0.25f * s;
        q.z = (m.m6 + m.m9) / s;
    }
    else {
        float s = sqrtf(1.0f + m.m10 - m.m0 - m.m5) * 2;
        q.w = (m.m4 - m.m1) / s;
        q.x = (m.m2 + m.m8) / s;
        q.y = (m.m6 + m.m9) / s;
        q.z = 0.25f * s;
    }

    return q;
}

void CreateMech(Mech* mech, int player)
{
    float roll = player % 2 == 0 ? 0.0f : PI;
    Quaternion rotation = QuaternionFromEuler(0.0f, 0.0f, roll);

    mech->legs_rotation = rotation;
    mech->torso_rotation = rotation;
    mech->legs_rotation_goal = rotation;
    mech->torso_rotation_goal = rotation;
	mech->projectile.rotation = rotation;
    mech->material = LoadMaterialDefault();
    mech->material.maps[MATERIAL_MAP_DIFFUSE].color = GRAY;
	mech->projectile.active = false;
    mech->player = player;
	mech->projectile.projectileMaterial = LoadMaterialDefault();

    mech->projectile.vel = { 0,0,0 };
}

void DestroyMech(Mech* mech)
{
    UnloadMaterial(mech->material);
}
Vector3 Vector3Cross(Vector3 a, Vector3 b) {
    return {
        a.y* b.z - a.z * b.y,
            a.z* b.x - a.x * b.z,
            a.x* b.y - a.y * b.x
    };
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

        if (IsGamepadButtonPressed(mech.player, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        {
            if (!mech.projectile.active)
            {
                mech.projectile.active = true;
                mech.projectile.pos = mech.pos;

                float forward_speed = 100.0f;
                float upward_speed = 200.0f;

                Vector3 localForward = { 0.0f, 1.0f, 0.0f };
                Matrix rotMat = QuaternionToMatrix(mech.torso_rotation);
                Vector3 worldDir = Vector3Transform(localForward, rotMat);

                Vector3 velocity = {
                    worldDir.x * forward_speed,
                    worldDir.y * forward_speed,
                    upward_speed
                };

                mech.projectile.vel = velocity;


                Vector2 dir2D = Vector2Normalize({ velocity.x, velocity.y });
                float roll = Vector2Angle(Vector2UnitY, dir2D);
                mech.projectile.rotation = QuaternionFromEuler(0.0f, 0.0f, roll);
            }
        }




        if (IsGamepadButtonPressed(mech.player, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
        {
            mech.projectile.active = false;
            mech.projectile.vel = { 0,0,0 };
        }
    }


    mech.legs_rotation = QuaternionRotateTowards(mech.legs_rotation, mech.legs_rotation_goal, 250.0f * DEG2RAD * dt);
    mech.torso_rotation = QuaternionRotateTowards(mech.torso_rotation, mech.torso_rotation_goal, 100.0f * DEG2RAD * dt);

    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;


    if (mech.projectile.active) {
        // Update projectile physics
        mech.projectile.vel.z -= 980.0f * dt;
        mech.projectile.pos.x += mech.projectile.vel.x * dt;
        mech.projectile.pos.y += mech.projectile.vel.y * dt;
        mech.projectile.pos.z += mech.projectile.vel.z * dt;
        mech.projectile.vel.x *= 0.99f;
        mech.projectile.vel.y *= 0.99f;

        Vector3 forward = Normalize(mech.projectile.vel);

        // Choose a vector not parallel to forward for basis
        Vector3 tmp = (fabsf(forward.z) > 0.99f) ?  Vetct{ 1, 0, 0 } : { 0, 0, 1 };

        Vector3 right = Normalize(Cross(tmp, forward));
        Vector3 up = Cross(forward, right);

        // Build rotation matrix with columns right, up, forward
        Matrix rotMat = {
            right.x,   right.y,   right.z,   0,
            up.x,      up.y,      up.z,      0,
            forward.x, forward.y, forward.z, 0,
            0,         0,         0,         1
        };

        Quaternion targetRotation = QuatFromMatrix(rotMat);

        // Smoothly rotate projectile.rotation toward targetRotation
        float maxDegreesPerSecond = 360.0f; // adjust as needed
        mech.projectile.rotation = QuaternionRotateTowards(
            mech.projectile.rotation,
            targetRotation,
            maxDegreesPerSecond * DEG2RAD * dt
        );
    }




}

void DrawMech(const Mech& mech)
{
    // RHS positive rotation is CCW
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);
	Matrix grenade_translation = MatrixTranslate(mech.projectile.pos.x, mech.projectile.pos.y, mech.projectile.pos.z);

    Matrix torso_rotation = QuaternionToMatrix(mech.torso_rotation);
    Matrix legs_rotation = QuaternionToMatrix(mech.legs_rotation);
	Matrix grenade_rotation = QuaternionToMatrix(mech.projectile.rotation);

    Matrix torso_world = torso_rotation * translation;
    Matrix legs_world = legs_rotation * translation;
	Matrix grenade_world = grenade_rotation * grenade_translation;
    DrawMesh(*g_meshes.mech_torso, mech.material, torso_world);
    DrawMesh(*g_meshes.mech_legs, mech.material, legs_world);
	if (mech.projectile.active)
	{
		
		DrawMesh(*g_meshes.gear_grenade, mech.projectile.projectileMaterial, grenade_world);
	}
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxesDebug(mech.pos, QuaternionToMatrix(mech.torso_rotation), 25.0f, 10.0f);
}
