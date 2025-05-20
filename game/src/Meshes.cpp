#include "Meshes.h"

Meshes g_meshes;

static Model f_mech_torso;
static Model f_mech_legs;
static Model f_bld_td;

void LoadMeshes()
{
	f_mech_torso = LoadModel("./assets/meshes/mech_torso.obj");
	f_mech_legs = LoadModel("./assets/meshes/mech_legs.obj");
	f_bld_td = LoadModel("./assets/meshes/bld_td.obj");

	g_meshes.mech_torso = &f_mech_torso.meshes[0];
	g_meshes.mech_legs = &f_mech_legs.meshes[0];
	g_meshes.bld_td = &f_bld_td.meshes[0];
}

void UnloadMeshes()
{
	g_meshes.bld_td = nullptr;
	g_meshes.mech_legs = nullptr;
	g_meshes.mech_torso = nullptr;

	UnloadModel(f_mech_legs);
	UnloadModel(f_mech_torso);
}
