#pragma once
#include "raylib.h"
#include "raymathext.h"
#include "Gear.h"

struct World;
struct Mech
{
    Vector3 pos = Vector3Zeros;
    Vector3 vel = Vector3Zeros;

    float turn_speed = 100.0f * DEG2RAD;
    float move_speed = 100.0f;
    float drag = 0.05f; // 0.0 = full drag, 1.0 = no drag

    Quaternion legs_rotation;
    Quaternion torso_rotation;
    Quaternion legs_rotation_goal;
    Quaternion torso_rotation_goal;

    Material material;

    int player = 0;

    Gear gear[4];
};

void CreateMech(Mech* mech, int player);
void DestroyMech(Mech* mech);

void UpdateMech(Mech& mech, World& world);
void DrawMech(const Mech& mech);
void DrawMechDebug(const Mech& mech);
