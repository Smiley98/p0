#pragma once
#include "raylib.h"

struct Shaders
{
    Shader skinning;
};

extern Shaders gShaders;

void LoadShaders();
void UnloadShaders();