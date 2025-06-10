#pragma once
#include "Scene.h"
class GroundScene : public Scene
{
public:
	void OnLoad() final;
	void OnUnload() final;

	void OnStart() final;
	void OnStop() final;

	void OnUpdate()final;
	void OnDraw() final;

	void OnDrawDebug() final;
	void OnDrawGui() final;
};
