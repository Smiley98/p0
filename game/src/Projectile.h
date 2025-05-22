#pragma once
#include "raylib.h"
#include "raymathext.h"
#include "Types.h"

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
		};
	};
};

// Inline creation for now. Factories will get very complicated very fast!
//void CreateProjectile(Projectile* p);
//void DestroyProjectile(Projectile* p);

void UpdateProjectile(Projectile& p);
void DrawProjectile(const Projectile& p);
void DrawProjectileDebug(const Projectile& p);