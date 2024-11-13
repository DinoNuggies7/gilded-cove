#include "texture.h"
#include <stdio.h>

Texture2D texture[TEXTURES];

void LoadTextures() {
	char path[256];
	for (int i = 0; i < TEXTURES; i++) {
		sprintf(path, "res/texture/%d.png", i);
		texture[i] = LoadTexture(path);
	}
}

void UnloadTextures() {
	for (int i = 0; i < TEXTURES; i++)
		UnloadTexture(texture[i]);
}

Rectangle GetTextureSrcRect(int id, int src) {
	src -= 1;
	Rectangle source = {(src * 96) % texture[id].width, (int)((src * 96) / texture[id].width) * 96, 96, 96};
	return source;
}
