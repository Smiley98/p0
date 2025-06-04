#pragma once
#include "WorldDef.h"
#include "ParticleEmitter.h"

struct Projectile
{
	ProjectileType type = PROJECTILE_TYPE_COUNT;

	Vector3 pos = Vector3Zeros;
	Vector3 vel = Vector3Zeros;

	float radius = 0.0f;
	float length = 0.0f;

	Color color = WHITE;
	Material material;
	Mesh* mesh = nullptr;

	float gravity_scale = 0.0f;
	float restitution = 0.0f;
	bool destroy = false;

	OnCollisionMechProjectile on_collision_mech = nullptr;
	OnCollisionProjectileBuilding on_collision_building = nullptr;

	ParticleEmitter trail;

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