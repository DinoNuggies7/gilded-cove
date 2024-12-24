#include "object.h"
#include "texture.h"
#include "player.h"
#include "draw.h"

#include <raymath.h>

void ObjectInit_Ladder(Object* this) {
	ActorInit(&this->actor);
	this->actor.textureID = TEX_LADDER;
}

void ObjectUpdate_Ladder(Object* this) {
	if (!this->data[1]) {
		switch (this->data[0]) {
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
		this->data[1] = true;
	}
	ActorUpdate(&this->actor);
}

void ObjectInteract_Ladder(Object* this) {
	if (player.isClimbing == 0) {
		player.isClimbing = this->id;
		player.camera.position = this->actor.pos;
		switch (this->data[0]) {
			case 0:
				player.camera.position.z += 0.3;
				break;
			case 1:
				player.camera.position.x -= 0.3;
				player.camera.position.z += 0.8;
				break;
			case 2:
				player.camera.position.x -= 0.8;
				player.camera.position.z += 0.29;
				break;
			case 3:
				player.camera.position.x -= 0.29;
				break;
		}
	}
	else {
		player.isClimbing = 0;
	}
}

void ObjectDraw_Ladder(Object* this) {
	Camera camera = player.camera;
	camera.up = (Vector3){0, 1, 0};
	camera.position = camera.target = this->actor.pos;
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
	DrawBillboardBetter(camera, texture[this->actor.textureID], (Rectangle){0, 0, texture[this->actor.textureID].width, texture[this->actor.textureID].height}, this->actor.pos, (Vector3){0, 1, 0}, this->actor.size, (Vector2){0.5, 0.5}, 0, WHITE);
}

void __attribute__((constructor)) CreateLadderObject() {
	ObjectInit[OBJ_LADDER] = &ObjectInit_Ladder;
	ObjectUpdate[OBJ_LADDER] = &ObjectUpdate_Ladder;
	ObjectInteract[OBJ_LADDER] = &ObjectInteract_Ladder;
	ObjectDraw[OBJ_LADDER] = &ObjectDraw_Ladder;
}