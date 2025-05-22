#include "Mech.h"
#include "DebugDraw.h"
#include "Meshes.h"
#include "World.h"

void FireRifle(Mech& mech, World& world)
{
    Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);

    Projectile p;
    p.pos = mech.pos + mech_dir * 10.0f;
    p.vel = mech_dir * 20.0f;
    p.radius = 2.0f;
    p.length = 8.0f;
    p.type = PROJECTILE_RIFLE;

    world.projectiles.push_back(p);
}

void FireShotgun(Mech& mech, World& world)
{
    Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);

    Projectile projectiles[3];
    for (int i = 0; i < 3; i++)
    {
        Projectile& p = projectiles[i];
        Quaternion spread = QuaternionFromEuler(0.0f, 0.0f, (-30.0f + i * 30.0f) * DEG2RAD);
        Vector3 dir = Vector3RotateByQuaternion(mech_dir, spread);

        p.pos = mech.pos + mech_dir * 10.0f;
        p.vel = dir * 15.0f;
        p.radius = 3.0f;
        p.length = 6.0f;
        p.type = PROJECTILE_SHOTGUN;

        world.projectiles.push_back(p);
    }
}

void FireGrenade(Mech& mech, World& world)
{
    // In addition to rotation test, we need some sort of system to queue grenades
    // *grenade launcher begin*
    //      - fire grenade 1
    //      - wait 0.2 seconds
    //      - fire grenade 2
    //      - wait 0.2 seconds
    //      - fire grenade 3
    //      - wait 0.2 seconds
    //      - fire grenade 4
    //      - wait 0.2 seconds
    // *grenade launcher end*

    Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);
    Vector3 dir = Vector3RotateByQuaternion(Vector3UnitY, QuaternionMultiply(mech.torso_rotation, QuaternionFromEuler(80.0f * DEG2RAD, 0.0f, 0.0f)));

    Projectile p;
    p.pos = mech.pos + mech_dir * 10.0f;
    p.vel = dir * 20.0f;
    p.radius = 4.0f;
    p.type = PROJECTILE_GRENADE;

    world.projectiles.push_back(p);
}

void FireMachineGun(Mech& mech, World& world)
{

}

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

void UpdateMech(Mech& mech, World& world)
{
    float dt = GetFrameTime();

    if (IsGamepadAvailable(mech.player))
    {
        const float deadzone = 0.5f;
        float moveX = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_LEFT_X));
        float moveY = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_LEFT_Y));
        float aimX = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_RIGHT_X));
        float aimY = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_RIGHT_Y));
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

        if (IsGamepadButtonPressed(mech.player, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
        {
            TraceLog(LOG_INFO, "Slot 0");
            FireRifle(mech, world);
        }

        if (IsGamepadButtonPressed(mech.player, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
        {
            TraceLog(LOG_INFO, "Slot 1");
            FireShotgun(mech, world);
        }

        if (IsGamepadButtonPressed(mech.player, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
        {
            TraceLog(LOG_INFO, "Slot 2");
            FireGrenade(mech, world);
        }

        if (IsGamepadButtonPressed(mech.player, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
        {
            TraceLog(LOG_INFO, "Slot 3");
            FireMachineGun(mech, world);
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

    Matrix torso_world = torso_rotation * translation;
    Matrix legs_world = legs_rotation * translation;

    DrawMesh(*g_meshes.mech_torso, mech.material, torso_world);
    DrawMesh(*g_meshes.mech_legs, mech.material, legs_world);
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxesDebug(mech.pos, QuaternionToMatrix(mech.torso_rotation), 25.0f, 10.0f);
}
