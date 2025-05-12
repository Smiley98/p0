#include "Mech.h"

void LoadMech()
{
    gMech.material = LoadMaterialDefault();
    gMech.material.maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
    gMech.torso = LoadModel("./assets/meshes/mech_torso.obj");
    gMech.legs = LoadModel("./assets/meshes/mech_legs.obj");

    gMech.pos = Vector3Zeros;
    gMech.roll = 0.0f;
}

void UnloadMech()
{
    UnloadModel(gMech.legs);
    UnloadModel(gMech.torso);
    UnloadMaterial(gMech.material);
}

void UpdateMech(Mech& mech)
{
    mech.roll = GetTime() * 100.0f * DEG2RAD;
}

void DrawMech(const Mech& mech)
{
    Matrix rotation = MatrixRotateZ(mech.roll);
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);
    Matrix world = rotation * translation;

    DrawMesh(mech.legs.meshes[0], mech.material, world);
    DrawMesh(mech.torso.meshes[0], mech.material, world);
}
