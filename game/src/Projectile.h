#pragma once
#include "raylib.h"
#include "raymathext.h"
#include "Types.h"

struct Mech;
struct World;
struct Projectile
{
	ProjectileType type = PROJECTILE_TYPE_COUNT;

	Vector3 pos;
	Vector3 vel;

	float radius; // <-- always used
	float length; // <-- only used for capsules

	// Not sure if we need to store rotation. May be able to get away with MatrixLookRotation(p.vel)
	//Quaternion rotation;

	//Material material;
	//Mesh* mesh;

	union
	{
		struct
		{
			float speed;
			float lifetime;
		} straight;

		struct
		{
			float gravity_scale;
			float restitution; // <-- bouncing grenade!?!?
		} grenade;

		struct
		{
			float move_speed;
			float turn_speed;
			uint32_t target_id;
		} missile;
	};
};

// Add this once we start rendering projectiles and we need to delete their materials
//void DestroyProjectile(Projectile* p);

void UpdateProjectile(Projectile& p);
void DrawProjectile(const Projectile& p);
void DrawProjectileDebug(const Projectile& p);

void CreateProjectileRifle(Mech& mech, World& world);
void CreateProjectileShotgun(Mech& mech, World& world);
void CreateProjectileGrenade(Mech& mech, World& world);
void CreateProjectileMissile(Mech& mech, World& world);

// Will probably need to add more arguments to projectile creation functions
// Also not sure if creation functions should be responsible for adding them to world,
// but its more convenient than looping through 3 shotgun projectiles externally...
