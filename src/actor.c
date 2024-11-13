#include "actor.h"
#include "texture.h"
#include "player.h"
#include "draw.h"
#include "map.h"

void ActorInit(Actor* this) {
	this->textureID = 0;
	this->srcrect = (Rectangle){0, 0, 96, 96};
	this->size = (Vector2){1, 1};
	this->color = WHITE;
}

void ActorUpdate(Actor* this) {
  
}

void ActorDraw(Actor* this) {
	DrawBillboardBetter(player.camera, texture[this->textureID], (Rectangle){0, 0, texture[this->textureID].width, texture[this->textureID].height}, (Vector3){this->pos.x, 0.5, this->pos.y}, (Vector3){0, 1, 0}, this->size, (Vector2){0.5, 0.5}, 0, WHITE);
}
