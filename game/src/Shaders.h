#pragma once
#include "raylib.h"

struct Shaders
{
    Shader skinning;
};

extern Shaders g_shaders;

void LoadShaders();
void UnloadShaders();