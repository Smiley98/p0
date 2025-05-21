#pragma once
#include "Scene.h"

class GearScene : public Scene
{
	void OnLoad() final;
	void OnUnload() final;

	void OnStart() final;
	void OnStop() final;

	void OnUpdate()final;
	void OnDraw() final;

	void OnDrawDebug() final;
	void OnDrawGui() final;
};

