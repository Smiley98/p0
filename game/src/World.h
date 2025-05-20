#pragma once
#include "raylib.h"
#include "raymathext.h"

#include "Mech.h"
#include "Building.h"
#include "Projectile.h"

#include <vector>

using Mechs = std::vector<Mech>;
using Buildings = std::vector<Building>;
using Projectiles = std::vector<Projectile>;
// using Walls = std::array<Wall, 4>; <-- waiting on plane collision support

struct World
{
	Mechs mechs;
	Buildings buildings;
	Projectiles projectiles;
	// Walls walls;
};

void LoadWorld(World& world);
void UnloadWorld(World& world);

void UpdateWorld(World& world);
void DrawWorld(const World& world);
void DrawWorldDebug(const World& world);
