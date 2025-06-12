#pragma once

#include "objects.h"

typedef struct {
  int x, y, type;
} Object;

extern Object *object;
extern int objects;

void init_object(Object* object, int type);
extern void (*use_object[O_TYPES])(Object* object);

void spawn_objects(int num);
