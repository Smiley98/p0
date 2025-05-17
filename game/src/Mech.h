#pragma once
#include "raylib.h"
#include "raymathext.h"

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

    Quaternion legsRotation;
    Quaternion legsRotationGoal;

    Quaternion torsoRotation;
    Quaternion torsoRotationGoal;
};

extern Mech gMech;

void LoadMech();
void UnloadMech();

void UpdateMech(Mech& mech);
void DrawMech(const Mech& mech);
void DrawMechDebug(const Mech& mech);
