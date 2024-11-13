#pragma once

#include <raylib.h>

enum Textures {
	TEX_DUNGEON,
	TEX_SCHLONGMAN,
	TEX_BARREL,
	TEX_DEMON,
	TEX_TORCH,
	TEX_DOOR,
	TEXTURES
};
extern Texture2D texture[TEXTURES];

void LoadTextures();
void UnloadTextures();

Rectangle GetTextureSrcRect(int id, int src);
