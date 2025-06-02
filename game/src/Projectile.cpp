#include "Projectile.h"
#include "Meshes.h"
#include <cassert>
#include "DebugDraw.h"

#include "Mech.h"
#include "World.h"

void CreateProjectileRifle(Mech& mech, World& world)
{
	Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);

	Projectile p;
	p.pos = mech.pos + mech_dir * 10.0f;
	p.vel = mech_dir * 20.0f;
	p.radius = 1.5f;
	p.type = PROJECTILE_RIFLE;

	p.mesh = g_meshes.prj_straight;
	p.material = LoadMaterialDefault();
	p.material.maps[MATERIAL_MAP_DIFFUSE].color = RED;

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

		p.mesh = g_meshes.prj_straight;
		p.material = LoadMaterialDefault();
		p.material.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;

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
	p.radius = 2.0f;
	p.type = PROJECTILE_GRENADE;

	p.mesh = g_meshes.prj_grenade;
	p.material = LoadMaterialDefault();
	p.material.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

	world.projectiles.push_back(p);
}

void CreateProjectileMissile(Mech& mech, World& world)
{
	Vector3 mech_dir = Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);
	Vector3 dir = Vector3RotateByQuaternion(Vector3UnitY, QuaternionMultiply(mech.torso_rotation, QuaternionFromEuler(80.0f * DEG2RAD, 0.0f, 0.0f)));

	Projectile p;
	p.pos = mech.pos + mech_dir * 10.0f;
	p.vel = dir * 40.0f;
	p.radius = 2.0f;
	p.type = PROJECTILE_MISSILE;

	p.mesh = g_meshes.prj_missile;
	p.material = LoadMaterialDefault();
	p.material.maps[MATERIAL_MAP_DIFFUSE].color = GOLD;

	world.projectiles.push_back(p);
}

void UpdateProjectile(Projectile& p)
{
	float dt = GetFrameTime();

	if (p.type == PROJECTILE_GRENADE ||
		p.type == PROJECTILE_MISSILE)
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
	Vector3 dir = Vector3Normalize(p.vel);
	Vector3 top = p.pos + dir * p.length;
	Vector3 bot = p.pos - dir * p.length;

	Color color = p.material.maps[MATERIAL_MAP_DIFFUSE].color;
	color.a = 196;
	switch (p.type)
	{
	case PROJECTILE_RIFLE:
	case PROJECTILE_SHOTGUN:
	case PROJECTILE_GRENADE:
	case PROJECTILE_MISSILE:
		DrawSphere(p.pos, p.radius, color);
		break;
	}

	DrawLineDebug(p.pos, p.pos + dir * 20.0f, YELLOW, 4.0f);
	DrawAxesDebug(p.pos, MatrixLookRotation(dir), 10.0f, 2.0f);
}
