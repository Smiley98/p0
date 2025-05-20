#pragma once
#include "raylib.h"
#include "raymathext.h"
#include "Types.h"

#include <cassert>
#include <vector>

struct Projectile
{
	ProjectileType type;

	Vector3 pos;
	Vector3 vel;

	float radius; // <-- always used
	float length; // <-- only used for capsules

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

inline Mesh* ProjectileMesh(ProjectileType type)
{

}

inline Material* ProjectileMaterial(ProjectileType type)
{

}

