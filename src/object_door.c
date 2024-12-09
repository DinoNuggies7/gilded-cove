#include "object.h"
#include "texture.h"
#include "player.h"
#include "draw.h"
#include "map.h"

#include <raymath.h>

void ObjectInit_Door(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_DOOR;
}

void ObjectUpdate_Door(Object* this) {
	if (!this->data[4]) {
		switch (this->data[0]) {
			case 0:
				this->actor.pos.y += 0.5;
				break;
			case 1:
				this->actor.pos.x -= 0.5;
				this->actor.pos.y++;
				break;
			case 2:
				this->actor.pos.x--;
				this->actor.pos.y += 0.49;
				break;
			case 3:
				this->actor.pos.x -= 0.49;
				break;
		}
		this->data[4] = true;
	}
	ActorUpdate(&this->actor);
}

void ObjectInteract_Door(Object* this) {
	player.camera.position.x = this->data[2];
	player.camera.position.z = this->data[3];
	FreeMap();
	ParseMap(this->data[1]);
	player.light = CreateLight(LIGHT_POINT, player.camera.position, Vector3Zero(), (Color){2, 2, 2}, shader);
}

void ObjectDraw_Door(Object* this) {
	Camera camera = player.camera;
	camera.up = (Vector3){0, 1, 0};
	camera.position = camera.target = (Vector3){this->actor.pos.x, 0.5, this->actor.pos.y};
	switch (this->data[0]) {
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
	DrawBillboardBetter(camera, texture[this->actor.textureID], (Rectangle){0, 0, texture[this->actor.textureID].width, texture[this->actor.textureID].height}, (Vector3){this->actor.pos.x, 0.5, this->actor.pos.y}, (Vector3){0, 1, 0}, this->actor.size, (Vector2){0.5, 0.5}, 0, WHITE);
}

void __attribute__((constructor)) CreateDoorObject() {
	ObjectInit[OBJ_DOOR] = &ObjectInit_Door;
	ObjectUpdate[OBJ_DOOR] = &ObjectUpdate_Door;
	ObjectInteract[OBJ_DOOR] = &ObjectInteract_Door;
	ObjectDraw[OBJ_DOOR] = &ObjectDraw_Door;
}
