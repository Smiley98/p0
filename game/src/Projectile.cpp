#include "Projectile.h"
#include "Meshes.h"
#include <cassert>
#include "DebugDraw.h"

// Rendering everything as debug colliders so we don't have to "guess" what our physics are doing
//inline Mesh* ProjectileMesh(ProjectileType type)
//{
//	// Expand this later. Will need to use par_shapes to generate a capsule mesh based on our coll
//	return g_meshes.proj_bullet;
//}

// Worry about this later, just create a default material for now
//inline Material ProjectileMaterial(ProjectileType type)
//{
//
//}

inline Color ProjectileColor(ProjectileType type)
{
	Color color = BLACK;
	switch (type)
	{
	case PROJECTILE_RIFLE:
		color = RED;
		break;

	case PROJECTILE_SHOTGUN:
		color = GREEN;
		break;

	case PROJECTILE_GRENADE:
		color = BLUE;
		break;

	case PROJECTILE_TYPE_COUNT:
		assert(false, "Invalid projectile type");
		break;
	}
	return color;
}

//void CreateProjectile(Projectile* p)
//{
//}
//
//void DestroyProjectile(Projectile* p)
//{
//}

void UpdateProjectile(Projectile& p)
{
	float dt = GetFrameTime();

	if (p.type == PROJECTILE_GRENADE)
	{
		p.vel += Vector3UnitZ * -10.0f * dt;
	}

	p.pos += p.vel * dt;
}

void DrawProjectile(const Projectile& p)
{
	Color color = ProjectileColor(p.type);
	Vector3 dir = Vector3Normalize(p.vel);
	Vector3 top = p.pos + dir * p.length;
	Vector3 bot = p.pos - dir * p.length;
	
	switch (p.type)
	{
	case PROJECTILE_RIFLE:
	case PROJECTILE_SHOTGUN:
		DrawCapsule(top, bot, p.radius, 4, 4, color);
		break;
	case PROJECTILE_GRENADE:
		DrawSphere(p.pos, p.radius, color);
		break;
	}
}

void DrawProjectileDebug(const Projectile& p)
{
	Vector3 dir = Vector3Normalize(p.vel);
	DrawLineDebug(p.pos, p.pos + dir * 7.5f, ORANGE, 5.0f);
	DrawAxesDebug(p.pos, MatrixLookRotation(dir), 10.0f, 2.0f);
}
