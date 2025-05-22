#pragma once
#include "raylib.h"

struct Meshes
{
	// TODO - Store mech animations as morph targets
	Mesh* mech_torso;
	Mesh* mech_legs;

	// Testing with just the TD building for simplicity
	Mesh* bld_td;
	//Mesh* bld_bmo;
	//Mesh* bld_condo;
	//Mesh* bld_cn_tower;

	// TODO - Load these once world-systems work ;)
	//Mesh* gear_rifle;
	//Mesh* gear_shotgun;
	//Mesh* gear_grenade;

	// Rendering projectiles as colliders until gameplay is finished
	//Mesh* proj_bullet;
};

extern Meshes g_meshes;

void LoadMeshes();
void UnloadMeshes();
