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
	Vector2 dir = Vector2Normalize(Vector2Subtract(ppos, (Vector2){this->actor.pos.x, this->actor.pos.z}));
	static Vector3 vel;
	vel.x = dir.x;
	vel.z = dir.y;

	if (vel.y > -10)
		vel.y -= 10 * GetFrameTime();
	else
		vel.y = -10;

	float speed = 1.5;
	Vector3 nvel = {vel.x * speed * GetFrameTime(), vel.y * GetFrameTime(), vel.z * speed * GetFrameTime()};

	int col = DoCollision(this->actor.pos, &nvel, 0.2);
	if (col > 0)
		vel.y = 0;

	this->actor.pos.x += nvel.x;
	this->actor.pos.y += nvel.y;
	this->actor.pos.z += nvel.z;
	
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
