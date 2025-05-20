#include "Building.h"
#include "Meshes.h"
#include <cassert>

inline float BuildingDurability(BuildingType type)
{
    return 100.0f;
}

inline Color BuildingColor(const Building& building)
{
    Color src = LIGHTGRAY;
    Color dst = DARKGRAY;
    float t = building.durability / BuildingDurability(building.type);
    return ColorLerp(src, dst, t);
}

inline Mesh* BuildingMesh(BuildingType type)
{
    Mesh* mesh = nullptr;

    switch (type)
    {
    case BUILDING_TD:
        mesh = g_meshes.bld_td;
        break;
    }

    assert(mesh != nullptr, "Invalid building type");
    return mesh;
}

void CreateBuilding(Building* building, BuildingType type)
{
    building->type = type;
    building->durability = BuildingDurability(type);
    building->material = LoadMaterialDefault();
}

void DestroyBuilding(Building* building)
{
    UnloadMaterial(building->material);
    building->type = BUILDING_TYPE_COUNT;
}

void UpdateBuilding(Building& building)
{
    Color color = BuildingColor(building);
    building.material.maps[MATERIAL_MAP_DIFFUSE].color = color;
}

void DrawBuilding(const Building& building)
{
    DrawMesh(*BuildingMesh(building.type), building.material, MatrixTranslate(building.pos.x, building.pos.y, building.pos.z));
}
