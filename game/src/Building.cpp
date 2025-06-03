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
    building->radius = 5.0f;
    building->length = 50.0f;
    building->collision = false;
}

void DestroyBuilding(Building* building)
{
    UnloadMaterial(building->material);
    building->type = BUILDING_TYPE_COUNT;
}

void UpdateBuilding(Building& building)
{
    Color color = BuildingColor(building);
    color.a = 128;
    building.material.maps[MATERIAL_MAP_DIFFUSE].color = color;
}

void DrawBuilding(const Building& building)
{
    //DrawMesh(*BuildingMesh(building.type), building.material, MatrixTranslate(building.pos.x, building.pos.y, building.pos.z));
}

void DrawBuildingDebug(const Building& building)
{
    Vector3 bot = building.pos;
    Vector3 top = building.pos + Vector3UnitZ * (building.length - building.radius);
    // Game building colliders will have bot at pos so mech doesn't rebound downwards
    // (cylinder shape instead of dome shape)

    //Color cc = PURPLE;
    //Color cs = PINK;
    //cc.a = 128;
    //cs.a = 128;
    //DrawCapsule(bot, top, building.radius, 8, 4, cc);
    //DrawSphere(top, building.radius, cs);
    //DrawSphere(bot, building.radius, cs);
    DrawCapsule(bot, top, building.radius, 8, 4, building.collision ? RED : GREEN);
}
