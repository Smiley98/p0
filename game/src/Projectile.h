#pragma once
#include "WorldDef.h"

struct Projectile
{
	ProjectileType type = PROJECTILE_TYPE_COUNT;

	Vector3 pos;
	Vector3 vel;

	float radius;
	float length;

	Material material;
	Mesh* mesh;

	float gravity_scale = 0.0f;
	float restitution = 0.0f;
	bool destroy = false;

	OnCollisionMechProjectile on_collision_mech = nullptr;
	OnCollisionProjectileBuilding on_collision_building = nullptr;

#ifdef DEBUG
	bool debug_collion = false;
#endif

	union
	{
		struct
		{
			float speed;
			float lifetime;
		} straight;

		struct
		{
		} grenade;

		struct
		{
			float move_speed;
			float turn_speed;
			uint32_t target_id;
		} missile;
	};
};

void DestroyProjectile(Projectile* p);
void CreateProjectileRifle(Mech& mech, World& world);
void CreateProjectileShotgun(Mech& mech, World& world);
void CreateProjectileGrenade(Mech& mech, World& world);
void CreateProjectileMissile(Mech& mech, World& world, float roll);

void UpdateProjectile(Projectile& p);
void DrawProjectile(const Projectile& p);
void DrawProjectileDebug(const Projectile& p);