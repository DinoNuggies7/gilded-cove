#pragma once

#include "rlights.h"

typedef struct Map {
	int width, height;
	int* wall;
	int* floor;
	int* ceiling;
	int objects;
} Map;

extern Map map;

int GetWallTile(int x, int y);
int GetFloorTile(int x, int y);
int GetCeilingTile(int x, int y);

void DoCollision(Vector2 pos, Vector2* nvel, float size);

void ParseMap(int mapID);
void FreeMap();
