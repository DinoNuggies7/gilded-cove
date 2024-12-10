#include "object.h"
#include <stdlib.h>

int objects = 0;
Object* object;
void (*ObjectInit[OBJECTS])(Object* object);
void (*ObjectUpdate[OBJECTS])(Object* object);
void (*ObjectInteract[OBJECTS])(Object* object);
void (*ObjectDraw[OBJECTS])(Object* object);

Object* CreateObject(int type, Vector3 pos) {
	Object* obj = &object[objects];
	bool done = false;
	do {
		obj->id = rand();
		done = true;
		for (int i = 0; i < objects; i++) {
			if (object[i].id == obj->id) {
				done = false;
				break;
			}
		}
	} while (!done);
	obj->type = type;
	obj->init = ObjectInit[type];
	obj->update = ObjectUpdate[type];
	obj->interact = ObjectInteract[type];
	obj->draw = ObjectDraw[type];
	obj->actor.pos = pos;
	obj->init(obj);
	objects++;
	return obj;
}

void KillObject(int id) {
	int gid = -1;
	for (int i = 0; i < objects; i++) {
		if (object[i].id == id) {
			gid = i;
			break;
		}
	}
	if (gid > -1) {
		for (int i = gid; i < objects - 1; i++) {
			object[i] = object[i+1];
		}
		objects--;
	}
}
