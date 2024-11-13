#include "object.h"
#include "player.h"
#include "texture.h"
#include "map.h"
#include <raylib.h>
#include <raymath.h>

void ObjectInit_Demon(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_DEMON;
}

void ObjectUpdate_Demon(Object* this) {
	Vector2 ppos = (Vector2){player.camera.position.x, player.camera.position.z};
	Vector2 vel = Vector2Normalize(Vector2Subtract(ppos, this->actor.pos));

	float speed = 1.5;
	Vector2 nvel = {vel.x * speed * GetFrameTime(), vel.y * speed * GetFrameTime()};

	DoCollision(this->actor.pos, &nvel, 0.2);

	this->actor.pos.x += nvel.x;
	this->actor.pos.y += nvel.y;
	
	ActorUpdate(&this->actor);
}

void ObjectInteract_Demon(Object* this) {
	
}

void ObjectDraw_Demon(Object* this) {
	ActorDraw(&this->actor);
}

void __attribute__((constructor)) CreateDemonObject() {
	ObjectInit[OBJ_DEMON] = &ObjectInit_Demon;
	ObjectUpdate[OBJ_DEMON] = &ObjectUpdate_Demon;
	ObjectInteract[OBJ_DEMON] = &ObjectInteract_Demon;
	ObjectDraw[OBJ_DEMON] = &ObjectDraw_Demon;
}
