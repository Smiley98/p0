#include "MapScene.h"
#include "Camera.h"
#include "World.h"

static World f_world;

void MapScene::OnLoad()
{
	LoadWorld(f_world);
}

void MapScene::OnUnload()
{
	UnloadWorld(f_world);
}

void MapScene::OnStart()
{
}

void MapScene::OnStop()
{
}

void MapScene::OnUpdate()
{
	UpdateCamera();
	UpdateWorld(f_world);
}

void MapScene::OnDraw()
{
	DrawWorld(f_world);
}

void MapScene::OnDrawDebug()
{
	DrawWorldDebug(f_world);
}

void MapScene::OnDrawGui()
{

}
