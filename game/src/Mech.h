#pragma once
#include "raylib.h"
#include "p0math.h"

struct Mech
{
    Material material;
    Model torso;
    Model legs;

    Vector3 pos;
    Vector3 vel;

    float turnSpeed;
    float moveSpeed;

    // 0.0 = full drag, 1.0 = no drag
    float drag;

    float moveAngle;
    float moveAngleGoal;

    float aimAngle;
    float aimAngleGoal;
};

extern Mech gMech;

void LoadMech();
void UnloadMech();

void UpdateMech(Mech& mech);
void DrawMech(const Mech& mech);
void DrawMechDebug(const Mech& mech);
