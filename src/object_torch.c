#include "object.h"
#include "texture.h"
#include "draw.h"
#include "player.h"

#include <stdlib.h>
#include <raymath.h>

void ObjectInit_Torch(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_TORCH;
	this->data = malloc(sizeof(long) * 1);
	this->data[0] = (long long)CreateLight(LIGHT_POINT, Vector3Zero(), Vector3Zero(), BLACK, shader);
}

void ObjectUpdate_Torch(Object* this) {
	ActorUpdate(&this->actor);

	Vector2 offset = Vector2Normalize(Vector2Subtract((Vector2){player.camera.position.x, player.camera.position.z}, this->actor.pos));
	Light* _light = (Light*)this->data[0];
	float wick = sin(GetTime()) * 0.1;
	_light->position = (Vector3){this->actor.pos.x + offset.x * 0.1, 0.5, this->actor.pos.y + offset.y * 0.1};
	_light->color = (Color){2, 2, 2};
	// _light->attenuation = 1 + wick;
	_light->falloff = 1 + wick;
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
