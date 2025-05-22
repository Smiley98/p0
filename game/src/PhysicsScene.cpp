#include "PhysicsScene.h"
#include "Collision.h"
#include "Collision3D.h"
#include "Camera.h"

static Vector3 p0 = Vector3UnitX * -10.0f;
static Vector3 p1 = Vector3UnitX *  10.0f;
static float radius = 5.0f;
static bool collision = false;
static Camera camera;

void PhysicsScene::OnLoad()
{
	// fovy is screen top / bot when orthographic
	// (yMin = -50, yMax = 50, xMin = -50 * aspect, xMax = 50 * aspect)
	camera.position = Vector3UnitZ * 50.0f;
	camera.target = Vector3Zeros;
	camera.up = Vector3UnitY;
	camera.fovy = 100.0f;
	camera.projection = CAMERA_ORTHOGRAPHIC;
}

void PhysicsScene::OnUnload()
{
}

void PhysicsScene::OnStart()
{
}

void PhysicsScene::OnStop()
{
}

void PhysicsScene::OnUpdate()
{
	p0 = Vector3UnitX * sinf(GetTime()) * 10.0f;
	p1 = Vector3UnitX * cosf(GetTime()) * 10.0f;
	Vector3 v0{ p0.x, p0.y , p0.z};
	Vector3 v1{ p1.x, p1.y , p0.z};
	collision = SphereSphere(v0, radius, v1, radius);
}

void PhysicsScene::OnDraw()
{
	Color color = collision ? RED : GREEN;
	BeginMode3D(camera);
	DrawSphere(p0, radius, color);
	DrawSphere(p1, radius, color);
	EndMode3D();
}

void PhysicsScene::OnDrawDebug()
{
}

void PhysicsScene::OnDrawGui()
{

}
