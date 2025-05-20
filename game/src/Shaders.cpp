#include "Shaders.h"

Shaders g_shaders;

void LoadShaders()
{
	g_shaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
}

void UnloadShaders()
{
	UnloadShader(g_shaders.skinning);
}
