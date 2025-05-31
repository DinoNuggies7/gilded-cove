#include "map.h"
#include "draw.h"
#include "player.h"
#include "enemy.h"

#include <ncurses.h>

#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	generate_map(20, 20, 5);
	init_player();
	spawn_enemies(5);

	while (true) {
		clear();
		draw_map();
		draw_objects();
		mvprintw(map.h, 0, "Health %d/%d\n", player.hp, player.hp_max);
		refresh();

		update_player();

		for (int i = 0; i < enemies; i++) {
			if (enemy[i].hp > 0)
				update_enemy(&enemy[i]);
			else
				kill_enemy(i);
		}

		if (player.hp <= 0) break;
	}
	endwin();

	free(player.room);
	free_enemies();
	free_map();

	printf("You died! 'Tis was a good run.\n");
	return 0;
}
