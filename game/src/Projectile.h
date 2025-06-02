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

	float radius;
	float length;

	Material material;
	Mesh* mesh;

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

// TODO - Figure out how entities should be deleted
//void DestroyProjectile(Projectile* p);
void CreateProjectileRifle(Mech& mech, World& world);
void CreateProjectileShotgun(Mech& mech, World& world);
void CreateProjectileGrenade(Mech& mech, World& world);
void CreateProjectileMissile(Mech& mech, World& world);

void UpdateProjectile(Projectile& p);
void DrawProjectile(const Projectile& p);
void DrawProjectileDebug(const Projectile& p);
