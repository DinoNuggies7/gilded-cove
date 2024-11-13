#pragma once

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define FIRE (Color){50, 30, 10, 255}

extern Shader shader;

void DrawForeground();
void DrawBackground();

void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color);
void DrawBillboardBetter(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint);
