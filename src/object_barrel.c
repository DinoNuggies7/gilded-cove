#include "object.h"
#include "texture.h"

void ObjectInit_Barrel(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_BARREL;
}

void ObjectUpdate_Barrel(Object* this) {
	ActorUpdate(&this->actor);
}

void ObjectInteract_Barrel(Object* this) {
	
}

void ObjectDraw_Barrel(Object* this) {
	ActorDraw(&this->actor);
}

void __attribute__((constructor)) CreateBarrelObject() {
	ObjectInit[OBJ_BARREL] = &ObjectInit_Barrel;
	ObjectUpdate[OBJ_BARREL] = &ObjectUpdate_Barrel;
	ObjectInteract[OBJ_BARREL] = &ObjectInteract_Barrel;
	ObjectDraw[OBJ_BARREL] = &ObjectDraw_Barrel;
}
