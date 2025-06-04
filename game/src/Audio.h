#pragma once
#include "raylib.h"

struct Audio
{
	Sound fire_rifle;
	Sound fire_shotgun;
	Sound fire_grenade;
	Sound fire_dasher;
};

extern Audio g_audio;

void LoadAudio();
void UnloadAudio();
