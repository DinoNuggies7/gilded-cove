#include "object.h"
#include "texture.h"
#include "player.h"
#include "draw.h"
#include "map.h"
#include "raymath.h"

#include <cjson/cJSON.h>

void ObjectInit_Door(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_DOOR;

	cJSON* jdata = cJSON_Parse(this->data);
	cJSON* jrotation = cJSON_GetObjectItem(jdata, "rotation");
	switch ((int)cJSON_GetNumberValue(jrotation)) {
		case 0:
			this->actor.pos.z += 0.5;
			break;
		case 1:
			this->actor.pos.x -= 0.5;
			this->actor.pos.z++;
			break;
		case 2:
			this->actor.pos.x--;
			this->actor.pos.z += 0.49;
			break;
		case 3:
			this->actor.pos.x -= 0.49;
			break;
	}
	cJSON_Delete(jdata);
}

void ObjectUpdate_Door(Object* this) {
	ActorUpdate(&this->actor);
}

void ObjectInteract_Door(Object* this) {
	cJSON* jdata = cJSON_Parse(this->data);
	cJSON* jwarp = cJSON_GetObjectItem(jdata, "warp");
	cJSON* jpx = cJSON_GetObjectItem(jdata, "px");
	cJSON* jpy = cJSON_GetObjectItem(jdata, "py");
	
	FreeMap();
	ParseMap(cJSON_GetNumberValue(jwarp));
	player.camera.position.x = cJSON_GetNumberValue(jpx);
	player.camera.position.z = cJSON_GetNumberValue(jpy);
	player.light = CreateLight(LIGHT_POINT, player.camera.position, Vector3Zero(), (Color){2, 2, 2}, shader);

	cJSON_Delete(jdata);
}

void ObjectDraw_Door(Object* this) {
	cJSON* jdata = cJSON_Parse(this->data);
	cJSON* jrotation = cJSON_GetObjectItem(jdata, "rotation");
	
	Camera camera = player.camera;
	camera.up = (Vector3){0, 1, 0};
	camera.position = camera.target = this->actor.pos;
	switch ((int)cJSON_GetNumberValue(jrotation)) {
		case 0:
			camera.position.z += 0.1;
			break;
		case 1:
			camera.position.x -= 0.1;
			break;
		case 2:
			camera.position.z -= 0.1;
			break;
		case 3:
			camera.position.x += 0.1;
			break;
	}
	DrawBillboardBetter(camera, texture[this->actor.textureID], (Rectangle){0, 0, texture[this->actor.textureID].width, texture[this->actor.textureID].height}, this->actor.pos, (Vector3){0, 1, 0}, this->actor.size, (Vector2){0.5, 0.5}, 0, WHITE);

	cJSON_Delete(jdata);
}

void __attribute__((constructor)) CreateDoorObject() {
	ObjectInit[OBJ_DOOR] = &ObjectInit_Door;
	ObjectUpdate[OBJ_DOOR] = &ObjectUpdate_Door;
	ObjectInteract[OBJ_DOOR] = &ObjectInteract_Door;
	ObjectDraw[OBJ_DOOR] = &ObjectDraw_Door;
}
