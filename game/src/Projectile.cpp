#include "Projectile.h"
#include "Meshes.h"
#include <cassert>
#include "DebugDraw.h"

#include "Mech.h"
#include "World.h"

Color ProjectileColor(ProjectileType type);
Mesh* ProjectileMesh(ProjectileType type);
Material ProjectileMaterial(ProjectileType type);

void CreateProjectileRifle(Mech& mech, World& world)
{
	Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);

	Projectile p;
	p.pos = mech.pos + mech_dir * 10.0f;
	p.vel = mech_dir * 20.0f;
	p.radius = 1.5f;
	p.type = PROJECTILE_RIFLE;

	p.mesh = ProjectileMesh(p.type);
	p.material = ProjectileMaterial(p.type);

	world.projectiles.push_back(p);
}

void CreateProjectileShotgun(Mech& mech, World& world)
{
	Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);

	Projectile projectiles[3];
	for (int i = 0; i < 3; i++)
	{
		Projectile& p = projectiles[i];
		Quaternion spread = QuaternionFromEuler(0.0f, 0.0f, (-30.0f + i * 30.0f) * DEG2RAD);
		Vector3 dir = Vector3RotateByQuaternion(mech_dir, spread);

		p.pos = mech.pos + mech_dir * 10.0f;
		p.vel = dir * 15.0f;
		p.radius = 2.0f;
		p.type = PROJECTILE_SHOTGUN;

		p.mesh = ProjectileMesh(p.type);
		p.material = ProjectileMaterial(p.type);

		world.projectiles.push_back(p);
	}
}

void CreateProjectileGrenade(Mech& mech, World& world)
{
	Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);
	Vector3 dir = Vector3RotateByQuaternion(Vector3UnitY, QuaternionMultiply(mech.torso_rotation, QuaternionFromEuler(80.0f * DEG2RAD, 0.0f, 0.0f)));

	Projectile p;
	p.pos = mech.pos + mech_dir * 10.0f;
	p.vel = dir * 20.0f;
	p.radius = 4.0f;
	p.type = PROJECTILE_GRENADE;

	world.projectiles.push_back(p);
}

void CreateProjectileMissile(Mech& mech, World& world)
{

}

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
	Matrix t = MatrixTranslate(p.pos.x, p.pos.y, p.pos.z);
	Matrix r = MatrixLookRotation(Vector3Normalize(p.vel));
	DrawMesh(*p.mesh, p.material, r * t);
}

void DrawProjectileDebug(const Projectile& p)
{
	Color color = ProjectileColor(p.type);
	color.a = 196;
	Vector3 dir = Vector3Normalize(p.vel);
	Vector3 top = p.pos + dir * p.length;
	Vector3 bot = p.pos - dir * p.length;

	// Most projectiles are spheres since they behave like bullets
	//DrawCapsule(top, bot, p.radius, 4, 4, color);
	switch (p.type)
	{
	case PROJECTILE_RIFLE:
	case PROJECTILE_SHOTGUN:
	case PROJECTILE_GRENADE:
		DrawSphere(p.pos, p.radius, color);
		break;
	}

	DrawLineDebug(p.pos, p.pos + dir * 7.5f, ORANGE, 5.0f);
	DrawAxesDebug(p.pos, MatrixLookRotation(dir), 10.0f, 2.0f);
}

Color ProjectileColor(ProjectileType type)
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

	case PROJECTILE_MISSILE:
		color = GOLD;
		break;

	case PROJECTILE_TYPE_COUNT:
		assert(false, "Invalid projectile type");
		break;
	}
	return color;
}

Mesh* ProjectileMesh(ProjectileType type)
{
	Mesh* mesh = nullptr;
	switch (type)
	{
	case PROJECTILE_RIFLE:
		mesh = g_meshes.prj_straight;
		break;

	case PROJECTILE_SHOTGUN:
		mesh = g_meshes.prj_straight;
		break;

	case PROJECTILE_GRENADE:
		break;
	}

	assert(mesh != nullptr, "Invalid projectile type!");
	return mesh;
}

Material ProjectileMaterial(ProjectileType type)
{
	Material material = LoadMaterialDefault();
	material.maps[MATERIAL_MAP_DIFFUSE].color = ProjectileColor(type);
	return material;
}
