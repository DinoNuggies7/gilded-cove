#pragma once

#include "rlights.h"

typedef struct Map {
	int width, height, layers;
	int* wall;
	int objects;
} Map;

extern Map map;

int GetWallTile(int x, int y, int l);

int DoCollision(Vector3 pos, Vector3* nvel, float size);

void ParseMap(int mapID);
void FreeMap();
