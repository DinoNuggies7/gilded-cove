#include "object.h"

void use_loot(Object* this) {
	
}

void __attribute__((constructor)) construct_loot() {
	use_object[O_LOOT] = &use_loot;
}
