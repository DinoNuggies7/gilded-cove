#include "object.h"
#include "map.h"

#include <stdlib.h>

Object *object;
int objects = 0;

void init_object(Object* this, int type) {
	do {
		this->x = rand() % (map.w - 2) + 1;
		this->y = rand() % (map.h - 2) + 1;
	}
	while (map.data[this->x + this->y * map.w]);

	this->type = type;
}

void (*use_object[O_TYPES])(Object* object);

void spawn_objects(int num) {
	objects = num;
	object = malloc(objects * sizeof(Object));

	init_object(&object[0], O_EXIT);
	for (int i = 1; i < num; i++)
		init_object(&object[i], O_LOOT);
}
