#include "Building.h"
#include "Meshes.h"
#include "rlgl.h"
#include <cassert>

inline float BuildingDurability(BuildingType type)
{
    return 100.0f;
}

inline Color BuildingColor(const Building& building)
{
    Color src = LIGHTGRAY;
    Color dst = DARKGRAY;
    float t = 1.0f - (building.durability / BuildingDurability(building.type));
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

    case BUILDING_CONDO:
        mesh = g_meshes.bld_condo;
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
    building->radius = 5.0f;
    building->length = 25.0f;
    building->death_timer = 2.0f;
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

    float dt = GetFrameTime();
    if (building.durability <= 0.0f)
    {
        building.death_timer -= dt;
        building.pos.z -= 10.0f * dt;
    }

    building.destroy = building.death_timer <= 0.0f;
}

void DrawBuilding(const Building& building)
{
    DrawMesh(*BuildingMesh(building.type), building.material, MatrixTranslate(building.pos.x, building.pos.y, building.pos.z));
}

void DrawBuildingDebug(const Building& building)
{
    Vector3 bot = building.pos;
    Vector3 top = building.pos + Vector3UnitZ * (building.length - building.radius);
    Color color = building.debug_collion ? RED : building.material.maps[MATERIAL_MAP_DIFFUSE].color;
    color.a = 128;
    //DrawCapsule(bot, top, building.radius, 8, 4, color);

    // top & bot spheres vs capsule test
    //DrawCapsule(bot, top, building.radius, 8, 4, { 200, 122, 255, 128 });
    //DrawSphere(top, building.radius, { 0, 82, 172, 128 });
    //DrawSphere(bot, building.radius, { 0, 82, 172, 128 });
}
