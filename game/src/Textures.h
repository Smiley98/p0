#pragma once
#include "raylib.h"

struct Textures
{
	Texture2D white;
};

extern Textures g_textures;

void LoadTextures();
void UnloadTextures();
