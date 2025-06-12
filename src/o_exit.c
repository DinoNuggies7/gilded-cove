#include "object.h"
#include "player.h"
#include "enemy.h"
#include "map.h"

#include <ncurses.h>
#include <malloc.h>

void use_exit(Object* this) {
	printw("> go to next floor? (y/n)");
	if (getch() != 'y') return;

	free(player.map);
	free(player.room);
	free_enemies();
	free_map();

	player.floor++;

	generate_map(20, 20, 5);
	spawn_enemies(5);
	spawn_objects(1);
	init_player();
}

void __attribute__((constructor)) construct_exit() {
	use_object[O_EXIT] = &use_exit;
}
