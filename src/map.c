#include "map.h"
#include "object.h"
#include "draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

Map map;

int solidWallID[] = {1, 2, 4};

int GetWallTile(int x, int y, int l) {
	return map.wall[(x + y * map.width) + map.width * map.height * l];
}

int DoCollision(Vector3 pos, Vector3* nvel, float size) {
	int output = 0;
	int tile[4];
	int solids = sizeof(solidWallID) / sizeof(int);
	
	tile[0] = GetWallTile(pos.x + 0.5 - size + nvel->x, pos.z + 0.5 + size, pos.y - 0.5);
	tile[1] = GetWallTile(pos.x + 0.5 - size + nvel->x, pos.z + 0.5 - size, pos.y - 0.5);
	tile[2] = GetWallTile(pos.x + 0.5 + size + nvel->x, pos.z + 0.5 + size, pos.y - 0.5);
	tile[3] = GetWallTile(pos.x + 0.5 + size + nvel->x, pos.z + 0.5 - size, pos.y - 0.5);

	for (int i = 0; i < 4; i++) {
		bool solid = false;
		for (int j = 0; j < solids; j++) {
			if (tile[i] == solidWallID[j]) {
				solid = true;
				break;
			}
		}
		if (solid) {
			nvel->x = 0;
			break;
		}
	}

	tile[0] = GetWallTile(pos.x + 0.5 - size, pos.z + 0.5 + size, pos.y - 0.5 + nvel->y);
	tile[1] = GetWallTile(pos.x + 0.5 - size, pos.z + 0.5 - size, pos.y - 0.5 + nvel->y);
	tile[2] = GetWallTile(pos.x + 0.5 + size, pos.z + 0.5 + size, pos.y - 0.5 + nvel->y);
	tile[3] = GetWallTile(pos.x + 0.5 + size, pos.z + 0.5 - size, pos.y - 0.5 + nvel->y);

	for (int i = 0; i < 4; i++) {
		bool solid = false;
		for (int j = 0; j < solids; j++) {
			if (tile[i] == solidWallID[j]) {
				solid = true;
				break;
			}
		}
		if (solid) {
			output += 2;
			break;
		}
	}

	tile[0] = GetWallTile(pos.x + 0.5 - size, pos.z + 0.5 + size, pos.y + 0.1 + nvel->y);
	tile[1] = GetWallTile(pos.x + 0.5 - size, pos.z + 0.5 - size, pos.y + 0.1 + nvel->y);
	tile[2] = GetWallTile(pos.x + 0.5 + size, pos.z + 0.5 + size, pos.y + 0.1 + nvel->y);
	tile[3] = GetWallTile(pos.x + 0.5 + size, pos.z + 0.5 - size, pos.y + 0.1 + nvel->y);

	for (int i = 0; i < 4; i++) {
		bool solid = false;
		for (int j = 0; j < solids; j++) {
			if (tile[i] == solidWallID[j]) {
				solid = true;
				break;
			}
		}
		if (solid) {
			output += 1;
			break;
		}
	}

	tile[0] = GetWallTile(pos.x + 0.5 - size, pos.z + 0.5 - size + nvel->z, pos.y - 0.5);
	tile[1] = GetWallTile(pos.x + 0.5 - size, pos.z + 0.5 + size + nvel->z, pos.y - 0.5);
	tile[2] = GetWallTile(pos.x + 0.5 + size, pos.z + 0.5 - size + nvel->z, pos.y - 0.5);
	tile[3] = GetWallTile(pos.x + 0.5 + size, pos.z + 0.5 + size + nvel->z, pos.y - 0.5);

	for (int i = 0; i < 4; i++) {
		bool solid = false;
		for (int j = 0; j < solids; j++) {
			if (tile[i] == solidWallID[j]) {
				solid = true;
				break;
			}
		}
		if (solid) {
			nvel->z = 0;
			break;
		}
	}

	return output;
}

void ParseMap(int mapID) {
	char path[32];
	sprintf(path, "res/map/%d.tmj", mapID);

	FILE* file = fopen(path, "r");
	int ch = ' ';
	int size;
	for (size = 0; ch != '\0' && ch != EOF; size++)
		ch = fgetc(file);
	freopen(path, "r", file);
	
	char json_str[size];
	fread(json_str, sizeof(char), size, file);
	
	cJSON* json = cJSON_Parse(json_str);
	if (!json) puts("didn't read map");

	cJSON* mapwidth = cJSON_GetObjectItem(json, "width");
	cJSON* mapheight = cJSON_GetObjectItem(json, "height");
	map.width = cJSON_GetNumberValue(mapwidth);
	map.height = cJSON_GetNumberValue(mapheight);
	map.layers = 0;
	map.objects = 0;

	cJSON* layer, *layers = cJSON_GetObjectItem(json, "layers");
	cJSON_ArrayForEach(layer, layers) {
		cJSON* type = cJSON_GetObjectItem(layer, "type");
		if (strcmp(cJSON_GetStringValue(type), "tilelayer") == 0)
			map.layers++;
	}
	cJSON_ArrayForEach(layer, layers) {
		cJSON* type = cJSON_GetObjectItem(layer, "type");
		if (strcmp(cJSON_GetStringValue(type), "objectgroup") == 0) {
			cJSON* jobject, *jobjects = cJSON_GetObjectItem(layer, "objects");
			cJSON_ArrayForEach(jobject, jobjects) { map.objects++; }
		}
	}

	map.wall = (int*)malloc(sizeof(int) * (map.width * map.height * map.layers));
	for (int i = 0; i < map.width * map.height * map.layers; i++) {
		map.wall[i] = 0;
	}
	object = (Object*)malloc(sizeof(Object) * map.objects);

	cJSON_ArrayForEach(layer, layers) {
		cJSON* type = cJSON_GetObjectItem(layer, "type");
		cJSON* name = cJSON_GetObjectItem(layer, "name");
		int layernum = atoi(cJSON_GetStringValue(name));
		if (strcmp(cJSON_GetStringValue(type), "tilelayer") == 0) {
			cJSON* data = cJSON_GetObjectItem(layer, "data");
			cJSON* tile;
			int i = 0;
			cJSON_ArrayForEach(tile, data) {
				map.wall[i + map.width * map.height * layernum] = (int)cJSON_GetNumberValue(tile);
				i++;
			}
		}
		else if (strcmp(cJSON_GetStringValue(type), "objectgroup") == 0) {
			cJSON* jobject, *jobjects = cJSON_GetObjectItem(layer, "objects");

			int i = 0;
			cJSON_ArrayForEach(jobject, jobjects) {
				cJSON* objectX = cJSON_GetObjectItem(jobject, "x");
				cJSON* objectZ = cJSON_GetObjectItem(jobject, "y");
				cJSON* objectW = cJSON_GetObjectItem(jobject, "width");
				cJSON* objectH = cJSON_GetObjectItem(jobject, "height");
				cJSON* type = cJSON_GetObjectItem(jobject, "type");
				float x = cJSON_GetNumberValue(objectX) / cJSON_GetNumberValue(objectW);
				float z = cJSON_GetNumberValue(objectZ) / cJSON_GetNumberValue(objectH);
				Object* obj = CreateObject(atoi(cJSON_GetStringValue(type)), (Vector3){x, layernum + 0.5, z - 1});
				switch (obj->type) {
					case OBJ_DOOR:;
						cJSON* jrotation = cJSON_GetObjectItem(jobject, "rotation");
						cJSON* jwarp = cJSON_GetObjectItem(jobject, "name");
						cJSON* jproperties = cJSON_GetObjectItem(jobject, "properties");
						cJSON* jproperty;
						int px, py;
						cJSON_ArrayForEach(jproperty, jproperties) {
							cJSON* jpname = cJSON_GetObjectItem(jproperty, "name");
							cJSON* jpvalue = cJSON_GetObjectItem(jproperty, "value");
							if (strcmp(cJSON_GetStringValue(jpname), "px") == 0)
								px = cJSON_GetNumberValue(jpvalue);
							if (strcmp(cJSON_GetStringValue(jpname), "py") == 0)
								py = cJSON_GetNumberValue(jpvalue);
						}
						float rotation = cJSON_GetNumberValue(jrotation);
						int warp = atoi(cJSON_GetStringValue(jwarp));
						obj->data = malloc(sizeof(long long) * 5);
						obj->data[0] = (int)(rotation / 90);
						obj->data[1] = warp;
						obj->data[2] = px;
						obj->data[3] = py;
						obj->data[4] = false;
						break;
				}
				i++;
			}
		}
	}
	
	fclose(file);
	cJSON_Delete(json);
}

void FreeMap() {
	free(map.wall);
	free(object);
	map.objects = 0;
	objects = 0;
	lights = 0;
}
