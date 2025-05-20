#include "Textures.h"
#include "rlgl.h"

Textures g_textures;

void LoadTextures()
{
	{
        Texture2D& tex = g_textures.white;
        tex.width = 1;
        tex.height = 1;
        tex.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        tex.mipmaps = 1;
        unsigned char pixels[4] = { 255, 255, 255, 255 };
        unsigned int id = rlLoadTexture(pixels, tex.width, tex.height, tex.format, tex.mipmaps);
        tex.id = id;
	}
}

void UnloadTextures()
{
    rlUnloadTexture(g_textures.white.id);
}
