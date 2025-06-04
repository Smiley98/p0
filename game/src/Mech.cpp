#include "Mech.h"
#include "DebugDraw.h"
#include "Meshes.h"
#include "Camera.h"

#include "World.h"
#include <cassert>

void UpdateInputMove(Mech& mech);
void UpdateInputAim(Mech& mech);
void UpdateInputFire(Mech& mech, World& world);

void FireGear(Mech& mech, World& world, int slot);
void UpdateGear(Mech& mech, World& world, int slot);

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

    // Default loadout / testing
    mech->gear[0] = CreateGearRifle();
    mech->gear[1] = CreateGearShotgun();
    mech->gear[2] = CreateGearGrenadeLauncher();
    mech->gear[3] = CreateGearMissileLauncher();

    ParticleEmitter& pe = mech->trail;
    pe.spawn_rate = 10.0f;
    pe.lifetime = 2.0f;
    pe.speed = 10.0f;
    pe.size = 2.0f;
    pe.shape_type = PARTICLE_SHAPE_SPHERE;
    pe.shape.sphere.radius = 5.0f;
}

void DestroyMech(Mech* mech)
{
    UnloadMaterial(mech->material);
}

void UpdateMech(Mech& mech, World& world)
{
    if (!IsGamepadAvailable(mech.player))
    {
        TraceLog(LOG_WARNING, "Player %i gamepad polled but not found");
        return;
    }

    UpdateInputAim(mech);
    UpdateInputMove(mech);
    UpdateInputFire(mech, world);
    
    for (int i = 0; i < 4; i++)
        UpdateGear(mech, world, i);

    float dt = GetFrameTime();
    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;

    ParticleEmitter& pe = mech.trail;
    pe.position = mech.pos;
    pe.direction = TorsoDirection(mech) * -1.0f;
    UpdateParticleEmitter(pe);
}

void DrawMech(const Mech& mech)
{
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix torso_rotation = QuaternionToMatrix(mech.torso_rotation);
    Matrix legs_rotation = QuaternionToMatrix(mech.legs_rotation);

    Matrix torso_world = torso_rotation * translation;
    Matrix legs_world = legs_rotation * translation;

    DrawMesh(*g_meshes.mech_torso, mech.material, torso_world);
    DrawMesh(*g_meshes.mech_legs, mech.material, legs_world);
    DrawParticleEmitter(mech.trail, *GetCamera());
}

void DrawMechDebug(const Mech& mech)
{
    DrawAxesDebug(mech.pos, QuaternionToMatrix(mech.torso_rotation), 25.0f, 10.0f);
    Color color = mech.debug_collion ? SKYBLUE : mech.material.maps[MATERIAL_MAP_DIFFUSE].color;
    color.a = 128;
}

void UpdateInputAim(Mech& mech)
{
    const float deadzone = 0.1f;
    Vector2 input = Vector2Zeros;
    input.x = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_RIGHT_X));
    input.y = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_RIGHT_Y));
    input.x = fabsf(input.x) >= deadzone ? input.x : 0.0f;
    input.y = fabsf(input.y) >= deadzone ? input.y : 0.0f;
    input.y *= -1.0f;

    if (Vector2Length(input) >= deadzone)
    {
        Vector2 dir = Vector2Normalize(input);
        float roll = Vector2Angle(Vector2UnitY, dir);
        mech.torso_rotation_goal = QuaternionFromEuler(0.0f, 0.0f, roll);
        mech.torso_rotation = QuaternionRotateTowards(mech.torso_rotation, mech.torso_rotation_goal, 100.0f * DEG2RAD * GetFrameTime());
    }
}

void UpdateInputMove(Mech& mech)
{
    const float deadzone = 0.1f;
    Vector2 input = Vector2Zeros;
    input.x = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_LEFT_X));
    input.y = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_LEFT_Y));
    input.x = fabsf(input.x) >= deadzone ? input.x : 0.0f;
    input.y = fabsf(input.y) >= deadzone ? input.y : 0.0f;
    input.y *= -1.0f;

    if (Vector2Length(input) >= deadzone)
    {
        float dt = GetFrameTime();
        Vector2 dir = Vector2Normalize(input);
        float roll = Vector2Angle(Vector2UnitY, dir);
        mech.legs_rotation_goal = QuaternionFromEuler(0.0f, 0.0f, roll);
        mech.legs_rotation = QuaternionRotateTowards(mech.legs_rotation, mech.legs_rotation_goal, 250.0f * DEG2RAD * dt);

        mech.vel += Vector3{ dir.x, dir.y, 0.0f } * mech.move_speed * dt;
        mech.vel = Vector3Clamp(mech.vel, { -10.0f, -10.0f, 0.0f }, { 10.0f, 10.0f, 0.0f });
    }
}

void UpdateInputFire(Mech& mech, World& world)
{
    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        FireGear(mech, world, 0);
    }

    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
    {
        FireGear(mech, world, 1);
    }

    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    {
        FireGear(mech, world, 2);
    }

    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        FireGear(mech, world, 3);
    }
}

void FireGear(Mech& mech, World& world, int slot)
{
    Gear& gear = mech.gear[slot];
    if (gear.cooldown <= 0.0f)
    {
        TraceLog(LOG_INFO, "Slot %i", slot);
        gear.cooldown = gear.cooldown_max;
        switch (gear.type)
        {
        case GEAR_RIFLE:
            CreateProjectileRifle(mech, world);
            break;

        case GEAR_SHOTGUN:
            CreateProjectileShotgun(mech, world);
            break;

        case GEAR_GRENADE_LAUNCHER:
            gear.grenade_launcher.grenades = 6;
            break;

        case GEAR_MISSILE_LAUNCHER:
            gear.missile_launcher.missiles = 3;
            gear.missile_launcher.launch_roll = 20.0f * DEG2RAD;
            break;

        case GEAR_TYPE_COUNT:
            assert(false, "Invalid gear type!");
            break;
        }
    }
    // Don't need FireGearX functions yet. Either create projectile or defer to UpdateGear
}

void UpdateGear(Mech& mech, World& world, int slot)
{
    Gear& gear = mech.gear[slot];
    float dt = GetFrameTime();
    gear.cooldown -= dt;

    if (gear.type == GEAR_GRENADE_LAUNCHER)
    {
        GearGrenadeLauncher& g = gear.grenade_launcher;
        g.launch_cooldown -= dt;
        if (g.launch_cooldown <= 0.0f && g.grenades > 0)
        {
            CreateProjectileGrenade(mech, world);
            g.launch_cooldown = g.launch_cooldown_max;
            g.grenades--;
        }
    }
    else if (gear.type == GEAR_MISSILE_LAUNCHER)
    {
        GearMissileLauncher& g = gear.missile_launcher;
        g.launch_cooldown -= dt;
        if (g.launch_cooldown <= 0.0f && g.missiles > 0)
        {
            CreateProjectileMissile(mech, world, g.launch_roll);
            g.launch_roll -= 20.0f * DEG2RAD; // 20, 0, -20 
            g.launch_cooldown = g.launch_cooldown_max;
            g.missiles--;
        }
    }
}
