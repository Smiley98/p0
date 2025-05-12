#pragma once
#include "raylib.h"

// Animation proof of concept. May switch to morph targets since raylib doesn't do blending
struct MechAnimation
{
    Model model;
    ModelAnimation* clips = nullptr;    // Array of animation clips
    int count = 0;                      // Size of animation clips array

    unsigned int index = 0;     // Index of current animation (ie 0 = idle, 1 = walk, 2 = run etc)
    unsigned int frame = 0;     // Frame of current animation
};

extern MechAnimation gMechAnimation;

void LoadMechAnimation();
void UnloadMechAnimation();

void UpdateMechAnimation(MechAnimation& ma);
void DrawMechAnimation(const MechAnimation& ma, Matrix transform);
