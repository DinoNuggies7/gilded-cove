#include "object.h"
#include "texture.h"

void ObjectInit_Schlongman(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_SCHLONGMAN;
}

void ObjectUpdate_Schlongman(Object* this) {
	ActorUpdate(&this->actor);
}

void ObjectInteract_Schlongman(Object* this) {
	KillObject(this->id);
}

void ObjectDraw_Schlongman(Object* this) {
	ActorDraw(&this->actor);
}

void __attribute__((constructor)) CreateSchlongmanObject() {
	ObjectInit[OBJ_SCHLONGMAN] = &ObjectInit_Schlongman;
	ObjectUpdate[OBJ_SCHLONGMAN] = &ObjectUpdate_Schlongman;
	ObjectInteract[OBJ_SCHLONGMAN] = &ObjectInteract_Schlongman;
	ObjectDraw[OBJ_SCHLONGMAN] = &ObjectDraw_Schlongman;
}
