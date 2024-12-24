#pragma once

#include "actor.h"

typedef struct Object {
	int id;
	int type;
	Actor actor;
	char* data;
	void (*init)(struct Object* object);
	void (*update)(struct Object* object);
	void (*interact)(struct Object* object);
	void (*draw)(struct Object* object);
} Object;

enum Objects {
	OBJ_SCHLONGMAN,
	OBJ_BARREL,
	OBJ_DEMON,
	OBJ_TORCH,
	OBJ_DOOR,

	OBJECTS
};

Object* CreateObject(int type, Vector3 pos);
void KillObject(int id);

extern int objects;
extern Object* object;
extern void (*ObjectInit[OBJECTS])(Object* object);
extern void (*ObjectUpdate[OBJECTS])(Object* object);
extern void (*ObjectInteract[OBJECTS])(Object* object);
extern void (*ObjectDraw[OBJECTS])(Object* object);
