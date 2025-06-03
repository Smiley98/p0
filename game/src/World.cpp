#include "World.h"
#include "rlgl.h"
#include "Camera.h"
#include "Collision3D.h"

constexpr size_t MAX_MECHS = 4;
constexpr size_t MAX_BUILDINGS = 64;
constexpr size_t MAX_PROJECTILES = 256;

void LoadMechs(Mechs& mechs);
void LoadBuildings(Buildings& buildings);
void LoadProjectiles(Projectiles& projectiles);

void UnloadProjectiles(Projectiles& projectiles);
void UnloadBuildings(Buildings& buildings);
void UnloadMechs(Mechs& mechs);

void UpdateCollisions(World& world);
void UpdateCollisionsMechMech(Mechs& mechs);
void UpdateCollisionsMechBuilding(Mechs& mechs, Buildings& buildings);
void UpdateCollisionsMechProjectile(Mechs& mechs, Projectiles& projectiles);
void UpdateCollisionsProjectileBuilding(Projectiles& projectiles, Buildings& buildings);

void DrawGrid();
void DrawMechs(const Mechs& mechs);
void DrawBuildings(const Buildings& buildings);
void DrawProjectiles(const Projectiles& projectiles);

void LoadWorld(World& world)
{
	world.mechs.reserve(MAX_MECHS);
	world.buildings.reserve(MAX_BUILDINGS);
	world.projectiles.reserve(MAX_PROJECTILES);

    LoadMechs(world.mechs);
    LoadBuildings(world.buildings);
    LoadProjectiles(world.projectiles);
}

void UnloadWorld(World& world)
{
    UnloadProjectiles(world.projectiles);
    UnloadBuildings(world.buildings);
    UnloadMechs(world.mechs);

	world.projectiles.clear();
	world.buildings.clear();
	world.mechs.clear();
}

void UpdateWorld(World& world)
{
	UpdateCollisions(world);

    for (Mech& mech : world.mechs)
        UpdateMech(mech, world);

    for (Building& building : world.buildings)
        UpdateBuilding(building);

    for (Projectile& projectile : world.projectiles)
        UpdateProjectile(projectile);
}

void DrawWorld(const World& world)
{
    BeginMode3D(*GetCamera());
        DrawGrid();
        DrawMechs(world.mechs);
        DrawBuildings(world.buildings);
        DrawProjectiles(world.projectiles);
    EndMode3D();
}

void DrawWorldDebug(const World& world)
{
    // TODO - 2D vs 3D debug draw for lines vs text
    BeginMode3D(*GetCamera());

    for (const Mech& mech : world.mechs)
        DrawMechDebug(mech);

    for (const Building& building : world.buildings)
        DrawBuildingDebug(building);

    for (const Projectile& projectile : world.projectiles)
        DrawProjectileDebug(projectile);

    EndMode3D();
}

void LoadMechs(Mechs& mechs)
{
    Mech mech;
    CreateMech(&mech, 0);
    mechs.push_back(mech);
}

void LoadBuildings(Buildings& buildings)
{
    for (float y = -40.0f; y <= 40.0f; y += 20.0f)
    {
        for (float x = -80.0f; x <= 80.0f; x += 20.0f)
        {
            // Temporarily omit centre building for visibility
            if (x == 0.0f && y == 0.0f) continue;

            Building building;
            building.pos = { x, y, 0.0f };
            CreateBuilding(&building, BUILDING_TD);
            buildings.push_back(building);
        }
    }
}

void LoadProjectiles(Projectiles& projectiles)
{
}

void UnloadProjectiles(Projectiles& projectiles)
{
}

void UnloadBuildings(Buildings& buildings)
{
    for (Building& building : buildings)
        DestroyBuilding(&building);
}

void UnloadMechs(Mechs& mechs)
{
    for (Mech& mech : mechs)
        DestroyMech(&mech);
}

void UpdateCollisions(World& world)
{
	UpdateCollisionsMechMech(world.mechs);
	UpdateCollisionsMechBuilding(world.mechs, world.buildings);
	UpdateCollisionsMechProjectile(world.mechs, world.projectiles);
	UpdateCollisionsProjectileBuilding(world.projectiles, world.buildings);
}

void UpdateCollisionsMechMech(Mechs& mechs)
{
}

void UpdateCollisionsMechBuilding(Mechs& mechs, Buildings& buildings)
{
}

void UpdateCollisionsMechProjectile(Mechs& mechs, Projectiles& projectiles)
{
}

void UpdateCollisionsProjectileBuilding(Projectiles& projectiles, Buildings& buildings)
{
    for (Building& b : buildings)
        b.collision = false;

    for (size_t i = 0; i < projectiles.size(); i++)
    {
        for (size_t j = i + 1; j < buildings.size(); j++)
        {
            Projectile& p = projectiles[i];
            Building& b = buildings[j];

            //BUGFIX: Why does this succeed initially then fail after ~30 projectiles!?
            bool collision = SphereCapsule(p.pos, p.radius, b.pos + Vector3UnitZ * b.length * 0.5f, Vector3UnitZ, b.radius, b.length * 0.5f - b.radius);
            if (collision)
            {
                TraceLog(LOG_INFO, "Collision");
            }
            b.collision |= collision;
        }
    }

    if (IsKeyPressed(KEY_P))
    {
        TraceLog(LOG_INFO, "Projectile count: %i", projectiles.size());
    }
}

void DrawGrid()
{
    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(-50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();
}

void DrawMechs(const Mechs& mechs)
{
    for (const Mech& mech : mechs)
        DrawMech(mech);
}

void DrawBuildings(const Buildings& buildings)
{
    //rlEnableWireMode();
    for (const Building& building : buildings)
        DrawBuilding(building);
    //rlDisableWireMode();
}

void DrawProjectiles(const Projectiles& projectiles)
{
    for (const Projectile& projectile : projectiles)
        DrawProjectile(projectile);
}

// Collision cases:
// Mech-Mech
// Mech-Building
// Mech-Projectile
// Projectile-Building
// 
// Of those cases, only Mech-Projectile needs unique behaviour
// (ie is there an EMP projectile that disables the mech? Is their a mine that sends it airborn? etc)
