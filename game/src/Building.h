#pragma once
#include "WorldDef.h"

struct Building
{
	BuildingType type = BUILDING_TYPE_COUNT;
	Vector3 pos = Vector3Zeros;

	float radius;
	float length;

	Material material;
	float durability;
	float death_timer;

	bool destroy = false;

	OnCollisionMechBuilding on_collision_mech = nullptr;
	OnCollisionProjectileBuilding on_collision_projectile = nullptr;

#ifdef DEBUG
	bool debug_collion = false;
#endif
};

void CreateBuilding(Building* building, BuildingType type);
void DestroyBuilding(Building* building);

void UpdateBuilding(Building& building);
void DrawBuilding(const Building& building);
void DrawBuildingDebug(const Building& building);
