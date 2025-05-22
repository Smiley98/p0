#pragma once
#include "raylib.h"
#include "raymathext.h"
#include "Types.h"

struct Building
{
	BuildingType type = BUILDING_TYPE_COUNT;
	Vector3 pos = Vector3Zeros;

	Material material;
	float durability;
};

void CreateBuilding(Building* building, BuildingType type);
void DestroyBuilding(Building* building);

void UpdateBuilding(Building& building);
void DrawBuilding(const Building& building);
void DrawBuildingDebug(const Building& building);
