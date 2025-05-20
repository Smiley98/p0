#pragma once
#include "raylib.h"

// Mesh skinning proof of concept. Currently unused since no raylib support for animation blending.
// Will most likely implement animation rendering via custom morph-target renderer.
struct MechAnimation
{
    Model model;
    ModelAnimation* clips = nullptr;// Array of animation clips
    int count = 0;                  // Size of animation clips array

    unsigned int index = 0;         // Index of current animation (ie 0 = idle, 1 = walk, 2 = run etc)
    unsigned int frame = 0;         // Frame of current animation
};

void LoadMechAnimation(MechAnimation* ma);
void UnloadMechAnimation(MechAnimation* ma);

void UpdateMechAnimation(MechAnimation& ma);
void DrawMechAnimation(const MechAnimation& ma, Matrix transform);
