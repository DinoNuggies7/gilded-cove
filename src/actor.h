#pragma once

#include <raylib.h>

typedef struct Actor {
	int textureID;
	Rectangle srcrect;
	Vector3 pos;
	Vector2 size;
	Color color;
} Actor;

void ActorInit(Actor* actor);
void ActorUpdate(Actor* actor);
void ActorDraw(Actor* actor);
