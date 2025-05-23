#include "Meshes.h"

Meshes g_meshes;

static Model f_mech_torso;
static Model f_mech_legs;
static Model f_bld_td;
static Model f_temp_grenade;

void LoadMeshes()
{
	f_mech_torso = LoadModel("./assets/meshes/mech_torso.obj");
	f_mech_legs = LoadModel("./assets/meshes/mech_legs.obj");
	f_bld_td = LoadModel("./assets/meshes/bld_td.obj");
	//Grenade Load! 

	f_temp_grenade = LoadModel("./assets/meshes/tempgrenade.obj");
	g_meshes.gear_grenade = &f_temp_grenade.meshes[0]; 

	g_meshes.mech_torso = &f_mech_torso.meshes[0];
	g_meshes.mech_legs = &f_mech_legs.meshes[0];
	g_meshes.bld_td = &f_bld_td.meshes[0];
}

void UnloadMeshes()
{
	g_meshes.bld_td = nullptr;
	g_meshes.mech_legs = nullptr;
	g_meshes.mech_torso = nullptr;
	g_meshes.gear_grenade = nullptr;

	UnloadModel(f_mech_legs);
	UnloadModel(f_mech_torso);
	UnloadModel(f_bld_td);
	UnloadModel(f_temp_grenade);
}
