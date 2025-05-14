#pragma once
#include "raylib.h"
#include "raymath.h"

struct Mech
{
    Material material;
    Model torso;
    Model legs;

    Vector3 pos;
    Vector3 vel;

    float rollTorso;
    float rollLegs;

    float turnSpeed;
    float moveSpeed;

    float drag; // 0.0 = full drag, 1.0 = no drag
};

extern Mech gMech;

void LoadMech();
void UnloadMech();

void UpdateMech(Mech& mech);
void DrawMech(const Mech& mech);
void DrawMechDebug(const Mech& mech);
