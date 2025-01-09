#include "object.h"
#include "texture.h"
#include "draw.h"
#include "player.h"

#include <stdlib.h>
#include <raymath.h>
#include <cjson/cJSON.h>

void ObjectInit_Torch(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_TORCH;

	Light* light = CreateLight(LIGHT_POINT, Vector3Zero(), Vector3Zero(), BLACK, shader);

	cJSON* jdata = cJSON_CreateObject();
	cJSON_AddNumberToObject(jdata, "plight", (size_t)light);
	this->data = cJSON_Print(jdata);
	cJSON_Delete(jdata);
}

void ObjectUpdate_Torch(Object* this) {
	cJSON* jdata = cJSON_Parse(this->data);
	cJSON* jlight = cJSON_GetObjectItem(jdata, "plight");
	Light* plight = (Light*)(size_t)cJSON_GetNumberValue(jlight);
	
	ActorUpdate(&this->actor);

	Vector2 offset = Vector2Normalize(Vector2Subtract((Vector2){player.camera.position.x, player.camera.position.z}, (Vector2){this->actor.pos.x, this->actor.pos.z}));
	float wick = sin(GetTime()) * 0.1;
	plight->position = (Vector3){this->actor.pos.x + offset.x * 0.1, this->actor.pos.y, this->actor.pos.z + offset.y * 0.1};
	plight->color = (Color){2, 2, 2};
	// plight->attenuation = 1 + wick;
	plight->falloff = 1 + wick;

	cJSON_Delete(jdata);
}

void ObjectInteract_Torch(Object* this) {
	
}

void ObjectDraw_Torch(Object* this) {
	ActorDraw(&this->actor);
}

void __attribute__((constructor)) CreateTorchObject() {
	ObjectInit[OBJ_TORCH] = &ObjectInit_Torch;
	ObjectUpdate[OBJ_TORCH] = &ObjectUpdate_Torch;
	ObjectInteract[OBJ_TORCH] = &ObjectInteract_Torch;
	ObjectDraw[OBJ_TORCH] = &ObjectDraw_Torch;
}
