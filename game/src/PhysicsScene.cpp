#include "PhysicsScene.h"
#include "Collision.h"
#include "Camera.h"

static Vector3 p0 = Vector3UnitX * -10.0f;
static Vector3 p1 = Vector3UnitX *  10.0f;
static float radius = 5.0f;
static bool collision = false;
static Camera camera;

void PhysicsScene::OnLoad()
{
	// fovy is screen top / bot when orthographics
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
	Vector2 v0{ p0.x, p0.y };
	Vector2 v1{ p1.x, p1.y };
	collision = CircleCircle(v0, radius, v1, radius);
}

void PhysicsScene::OnDraw()
{
	Color color = collision ? RED : GREEN;
	BeginMode3D(camera);
	DrawSphere(p0, 5.0f, color);
	DrawSphere(p1, 5.0f, color);
	EndMode3D();
}

void PhysicsScene::OnDrawDebug()
{
}

void PhysicsScene::OnDrawGui()
{

}
