#pragma once
#include "raylib.h"
#include "raymath.h"

struct Mech
{
    Material material;
    Model torso;
    Model legs;

    Vector3 pos;
    float roll;
};

extern Mech gMech;

void LoadMech();
void UnloadMech();

void UpdateMech(Mech& mech);
void DrawMech(const Mech& mech);
