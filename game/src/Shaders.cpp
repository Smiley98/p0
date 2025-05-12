#include "Shaders.h"

Shaders gShaders;

void LoadShaders()
{
	gShaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
}

void UnloadShaders()
{
	UnloadShader(gShaders.skinning);
}
