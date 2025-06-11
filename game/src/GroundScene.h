#pragma once
#include "Scene.h"
class GroundScene : public Scene
{
public:
	Camera camera;
	Model model;
	Shader hexShader;
	RenderTexture2D hexGridTarget;

	// Shader uniforms
	int timeLoc, resLoc, fgColLoc, bgColLoc, hexResLoc, hexThicknessLoc;
	Vector3 fgColor = { 1.0f, 1.0f, 1.0f };
	Vector3 bgColor = { 0.0f, 0.0f, 0.0f };
	float hexRes = 10.0f;
	float hexThickness = 0.1f;
	float time = 0.0f;
	void OnLoad() final;
	void OnUnload() final;

	void OnStart() final;
	void OnStop() final;

	void OnUpdate()final;
	void OnDraw() final;

	void OnDrawDebug() final;
	void OnDrawGui() final;
};
